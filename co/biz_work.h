/**
* @file   biz_work.h
* @author sunpengcheng(sunpengcheng@foxmail.com)
* @date   2017-07-02 19:34:07
* @brief
**/
#ifndef _BIZ_WORK_H_
#define _BIZ_WORK_H_

#include <hiredis/hiredis.h>
#include <unistd.h>
#include <string.h>
#include "frame_work.h"
#include "thread_queue.h"

class BizWork {
 public:
  BizWork() {
    c = NULL;
  }

  ~BizWork() {
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

#endif
