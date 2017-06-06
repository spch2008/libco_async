/**
* @file   work_thread.h
* @author sunpengcheng(sunpengcheng@foxmail.com)
* @date   2017-06-04 18:43:48
* @brief
**/
#ifndef _WORK_THREAD_H_
#define _WORK_THREAD_H_

#include "co_routine.h"
#include "thread.h"
#include "thread_queue.h"

#include <stack>

struct Task {
  stCoRoutine_t* co;
  void* entry;
  int fd;
};

class WorkThread : public Thread {
 public:
  WorkThread(ThreadQueue<int>* thread_queue);
  virtual ~WorkThread();

  virtual void Do(int);
  void Handler();

  std::stack<Task*> task_list;
  ThreadQueue<int>* thread_queue;

 private:
  Task* main_task;
};

#endif
