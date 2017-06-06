/**
* @file   thread_data.h
* @author sunpengcheng(spch2008@foxmail.com)
* @date   2016-06-17 09:50:18
* @brief
**/

#ifndef _THREAD_DATA_H_
#define _THREAD_DATA_H_

#include <pthread.h>
#include <string>
#include "exception.h"
#include "thread_locker.h"

class ThreadDataException : public Exception
{
public:
    explicit ThreadDataException(const string &err) : Exception(err) {}
    ThreadDataException(const string &err, int code) : Exception(err, code) {}
    ~ThreadDataException() throw() {}
};

class ThreadDataManager
{
public:
    typedef pthread_key_t ThreadDataKey;

    class ThreadData
    {
    public:
        ThreadData() {}
        virtual ~ThreadData() {}
    };

    ThreadDataManager();
    ~ThreadDataManager();

    void SetData(ThreadData *data);
    void SetData(ThreadDataKey key, ThreadData *data);

    ThreadData *GetData();
    ThreadData *GetData(ThreadDataKey key);

    void DelData();
    void DelData(ThreadDataKey key);

protected:
    ThreadDataManager(const ThreadDataManager &);
    ThreadDataManager &operator=(const ThreadDataManager &);

    static void CreateKey();
    static void Destructor(void *data);

private:
    static pthread_once_t gOnceControl;
    static pthread_key_t gThreadKey;
};

#endif
