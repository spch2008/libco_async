/**
* @file   frame_work.cpp
* @author sunpengcheng(sunpengcheng@foxmail.com)
* @date   2017-06-04 18:43:48
* @brief
**/

#include "frame_work.h"

#include "biz_work.h"
#include "co_routine.h"
#include "co_routine_inner.h"

struct CoEntry {
  stCoRoutine_t* co;
  BizWork* biz_work;
  FrameWork* frame_work;
  int fd;
};

FrameWork::FrameWork(ThreadQueue<int>* thread_queue) {
  thread_queue_ = thread_queue;
}

void FrameWork::Do(int fd) {
}

bool FrameWork::TryPop(int* fd) {
  return thread_queue_->TryPop(fd);
}

CoEntry* FrameWork::GetCoEntry(std::list<CoEntry*>* free_list) {
  CoEntry* co_entry;
  if (free_list->empty()) {
    co_entry = new CoEntry;
    co_entry->biz_work = new BizWork;
    co_entry->frame_work = this;
    co_create(&(co_entry->co), NULL, WorkerHandler, co_entry);
  } else {
    co_entry = free_list->front();
    free_list->pop_front();
  }
  return co_entry;
}

void* FrameWork::WorkerHandler(void* arg) {
  co_enable_hook_sys();

  CoEntry* co_entry = reinterpret_cast<CoEntry*>(arg);
  BizWork* biz_work = co_entry->biz_work;
  biz_work->Do(co_entry->fd);

  return NULL;
}

void* FrameWork::MasterHandler(void* arg) {
  CoEntry* co_entry = reinterpret_cast<CoEntry*>(arg);
  FrameWork* frame_work = co_entry->frame_work;

  std::list<CoEntry*> used_entry_list;
  std::list<CoEntry*> free_entry_list;
  while (true) {
    int fd;
    if (used_entry_list.size() < 4 && frame_work->TryPop(&fd)) {
      CoEntry* ce = frame_work->GetCoEntry(&free_entry_list);
      used_entry_list.push_back(ce);

      ce->fd = fd;
      co_resume(ce->co);
    }

    struct pollfd pf = {0};
    pf.fd = -1;
    poll(&pf, 1, 1);

    frame_work->RemoveFinishedCo(&used_entry_list, &free_entry_list);
  }

  return NULL;
}

void FrameWork::RemoveFinishedCo(std::list<CoEntry*> *used_entry_list,
                                 std::list<CoEntry*> *free_entry_list) {
  std::list<CoEntry*>::iterator iter = used_entry_list->begin();
  while (iter != used_entry_list->end()) {
    CoEntry* co_entry = *iter;
    if (co_entry->co->cEnd) {
      co_entry->co->cEnd = 0;
      co_entry->co->cStart = 0;
      free_entry_list->push_back(co_entry);
      iter = used_entry_list->erase(iter);
    } else {
      ++iter;
    }
  }
}

void FrameWork::Handler() {
  CoEntry co_entry;
  co_entry.frame_work = this;

  co_create(&(co_entry.co), NULL, MasterHandler, &co_entry);
  co_resume(co_entry.co);
  co_eventloop(co_get_epoll_ct(), 0, 0);
}
