/**
* @file   work_thread.cpp
* @author sunpengcheng(sunpengcheng@foxmail.com)
* @date   2017-06-04 18:43:48
* @brief
**/

#include "work_thread.h"

WorkThread::WorkThread(ThreadQueue<int>* thread_queue) {
  this->thread_queue = thread_queue;
  main_task = new Task;
}

WorkThread::~WorkThread() {
  delete main_task;
  while (!task_list.empty()) {
    delete task_list.top();
    task_list.pop();
  }
}

void WorkThread::Do(int fd) {
}

static void* accept_routine(void* arg) {
  co_enable_hook_sys();
  Task* arg_task = (Task*)arg;
  WorkThread* worker = (WorkThread*)arg_task->entry;

  while (true) {
    int fd;
    if(worker->task_list.empty() || !worker->thread_queue->TryPop(&fd)) {
      struct pollfd pf = {0};
      pf.fd = -1;
      poll(&pf, 1, 1000);
      continue;
  }

  Task* task = worker->task_list.top();
  task->fd = fd;
  worker->task_list.pop();
  co_resume(task->co);
  }

  return NULL;
}

static void* work_routine(void *arg) {
  co_enable_hook_sys();

  Task* task = (Task*)arg;
  WorkThread* worker = (WorkThread*)task->entry;
  while (true) {
    if(task->fd == -1) {
      worker->task_list.push(task);
      co_yield_ct();
      continue;
    }

    int fd = task->fd;
    task->fd = -1;

    worker->Do(fd);
  }

  return NULL;
}

void WorkThread::Handler() {
  for(size_t i=0; i < 3; ++i) {
    Task* task = new Task;
    task->fd = -1;
    task->entry = this;

    co_create(&(task->co), NULL, work_routine, task);
    co_resume(task->co);
  }

  main_task->entry = this;
  co_create(&(main_task->co), NULL, accept_routine, main_task);
  co_resume(main_task->co);
  co_eventloop(co_get_epoll_ct(), 0, 0);
}
