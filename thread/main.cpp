/**
* @file   main.cpp
* @author sunpengcheng(sunpengcheng@foxmail.com)
* @date   2017-06-04 10:16:27
* @brief
**/
#include <errno.h>
#include <event.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>

#include <hiredis/hiredis.h>
#include <sys/socket.h>

#include "end_point.h"
#include "thread_queue.h"
#include "work_thread.h"

ThreadQueue<int> g_queue;
std::vector<Thread*> g_thread_pool;

class BizThread : public WorkThread {
 public:
  BizThread(ThreadQueue<int>* queue) : WorkThread(queue) {
    c = NULL;
  }

  ~BizThread() {
    redisFree(c);
  }

  void Do(int fd) {
    // Lazy init.
    if (!c) {
      c = redisConnect((char*)"127.0.0.1", 6381);
    }

    char buf[20];
    memset(buf, 0, sizeof(buf));

    read(fd, buf, sizeof(buf));
    redisReply* reply = (redisReply*)redisCommand(c, "GET %s", buf);
    write(fd, reply->str, reply->len);
    write(fd, "\n", 1);

    /* debug
    char buf[20] = "foo";
    redisReply* reply = (redisReply*)redisCommand(c, "GET %s", buf);
    write(fd, reply->str, reply->len);
    */
  }

 private:
  redisContext* c;
};

void Accept(int fd, short event, void *arg) {
  int sfd;
  struct sockaddr addr;
  socklen_t addrlen;

  while (true) {
    addrlen = sizeof(struct sockaddr);
    sfd = accept(fd, &addr, &addrlen);

    if (sfd == -1) {
      if (errno == EAGAIN) {
        break;
      } else {
        continue;
      }
    }

    g_queue.Push(sfd);
  } 
}

int main() {
  for (int i = 0; i < 4; ++i) {
    BizThread* thread = new BizThread(&g_queue);
    thread->Run();
    g_thread_pool.push_back(thread);
  }

  EndPoint end_point("127.0.0.1", "2017");
  end_point.CreateListenSocket();
  int listen_fd = end_point.GetListenFd();

  struct event ev;
  struct event_base *base = event_init();

  event_set(&ev, listen_fd, EV_PERSIST | EV_READ, Accept, NULL); 
  event_add(&ev, NULL); 

  write(1, "starting\n", 9);
  event_base_dispatch(base);
  return 0;
}
