/**
* @file   frame_main.cpp
* @author sunpengcheng(sunpengcheng@foxmail.com)
* @date   2017-06-04 10:16:27
* @brief
**/


#include <errno.h>
#include <event.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#include "biz_work.h"
#include "end_point.h"
#include "thread_queue.h"
#include "thread.h"

ThreadQueue<int> g_queue;

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
  std::vector<Thread*> thread_pool;
  for (int i = 0; i < 4; ++i) {
    BizWork* work = new BizWork;
    work->SetFdQueue(&g_queue);
    work->Run();
    thread_pool.push_back(work);
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
