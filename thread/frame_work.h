/**
* @file   frame_work.h
* @author sunpengcheng(sunpengcheng@foxmail.com)
* @date   2017-06-04 18:43:48
* @brief
**/
#ifndef _FRAME_WORK_H_
#define _FRAME_WORK_H_

#include "thread.h"
#include "thread_queue.h"

class FrameWork : public Thread {
 public:
  FrameWork();
  void SetFdQueue(ThreadQueue<int>* thread_queue);
  virtual void Do(int) = 0;

 private:
  void Handler();

  ThreadQueue<int>* thread_queue_;
};

#endif
