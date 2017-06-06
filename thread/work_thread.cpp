/**
* @file   work_thread.cpp
* @author sunpengcheng(sunpengcheng@foxmail.com)
* @date   2017-06-04 18:43:48
* @brief
**/

#include "work_thread.h"

WorkThread::WorkThread(ThreadQueue<int>* thread_queue) {
  thread_queue_ = thread_queue;
}

void WorkThread::Do(int fd) {
}

void WorkThread::Handler() {
  while (true) {
    int fd;
    if (thread_queue_->Pop(&fd)) {
      Do(fd);
    }
  }
}
