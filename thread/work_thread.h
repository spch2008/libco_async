/**
* @file   work_thread.h
* @author sunpengcheng(sunpengcheng@foxmail.com)
* @date   2017-06-04 18:43:48
* @brief
**/
#ifndef _WORK_THREAD_H_
#define _WORK_THREAD_H_

#include "thread.h"
#include "thread_queue.h"

class WorkThread : public Thread {
 public:
  WorkThread(ThreadQueue<int>* thread_queue);
  virtual void Do(int);

 private:
  void Handler();

  ThreadQueue<int>* thread_queue_;
};

#endif
