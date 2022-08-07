#ifndef mutex_hh_
#define mutex_hh_

#include "noCopy.hh"
#include <pthread.h>

class Mutex: public NoCopy{
//继承Nocopy禁止复制控制语义
public:
    Mutex();
    ~Mutex();
    void lock();
    void unlock();
    pthread_mutex_t* getMutexPtr(){ return &_mutex; }

private:
    pthread_mutex_t _mutex;
};   

//自动解锁--RAII
class MutexLockGuard{
public:
    MutexLockGuard(Mutex& mutex): _mutex(mutex){ _mutex.lock(); }
    ~MutexLockGuard(){  _mutex.unlock(); }
private:
    Mutex& _mutex;
};
#endif 
