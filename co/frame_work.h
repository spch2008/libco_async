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
#include <list>

class CoEntry;

class FrameWork : public Thread {
 public:
  FrameWork(ThreadQueue<int>* thread_queue);
  virtual void Do(int);

 private:
  static void* MasterHandler(void*);
  static void* WorkerHandler(void*);

  void RemoveFinishedCo(std::list<CoEntry*>*, std::list<CoEntry*>*);
  CoEntry* GetCoEntry(std::list<CoEntry*>*);

  bool TryPop(int* fd);
  void Handler();

  ThreadQueue<int>* thread_queue_;
};

#endif
