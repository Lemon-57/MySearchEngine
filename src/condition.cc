#include "../include/condition.hh"
#include "../include/mutex.hh"
#include <stdio.h>

Condition::Condition(Mutex& mutex)
: _mutex(mutex){
    if(pthread_cond_init(&_cond, nullptr)){
        perror("pthread_cond_init");
    }
}

Condition::~Condition(){
    if(pthread_cond_destroy(&_cond)){
        perror("pthread_cond_destroy");
    }
}

void Condition::wait(){
    if(pthread_cond_wait(&_cond, _mutex.getMutexPtr())){
        perror("pthread_cond_wait");
    }
}

void Condition::notify(){
    //注意虚假唤醒(signal至少唤醒一个)
    if(pthread_cond_signal(&_cond)){
        perror("pthread_cond_signal");
    }
}

void Condition::notifyAll(){
    if(pthread_cond_broadcast(&_cond)){
        perror("pthread_cond_broadcast");
    }
}