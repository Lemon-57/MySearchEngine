#include "../include/threadBO.hh"
#include <stdio.h>

Thread::Thread(ThreadCallback &&cb)
: _tid(0)
, _isRunning(false)
, _cb(std::move(cb))//注册
{   }

Thread::~Thread(){
    //本例析构之前_isRunning已被置为false
    if(_isRunning){
        //分离子线程，子线程自己回收自己的资源
        pthread_detach(_tid);
    }
}


void Thread::start(){
    int ret = pthread_create(&_tid, nullptr, threadFunc, this);
    if(ret){
        perror("pthread_create");
        return;
    }
    _isRunning = true;
}

void Thread::join(){
    if(_isRunning){
        int ret = pthread_join(_tid, nullptr);
        if(ret){
            perror("pthread_join");
            return;
        }
        _isRunning = false;
    }
}

void *Thread::threadFunc(void *arg){
    Thread *pth = static_cast<Thread *>(arg);
    if(pth){
        pth->_cb();
    }

    //返回值void*
    //return nullptr;
    pthread_exit(nullptr);
}