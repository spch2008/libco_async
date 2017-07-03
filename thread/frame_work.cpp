/**
* @file   frame_work.cpp
* @author sunpengcheng(sunpengcheng@foxmail.com)
* @date   2017-06-04 18:43:48
* @brief
**/

#include "frame_work.h"

FrameWork::FrameWork() {
}

void FrameWork::SetFdQueue(ThreadQueue<int>* thread_queue) {
  thread_queue_ = thread_queue;
}

void FrameWork::Do(int fd) {
}

void FrameWork::Handler() {
  while (true) {
    int fd;
    if (thread_queue_->Pop(&fd)) {
      Do(fd);
    }
  }
}
