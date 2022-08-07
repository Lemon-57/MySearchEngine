#include "../include/mutex.hh"
#include <stdio.h>

Mutex::Mutex(){
    if(pthread_mutex_init(&_mutex, nullptr)){
        perror("pthread_mutex_init");
    }
}

Mutex::~Mutex(){
    if(pthread_mutex_destroy(&_mutex)){
        perror("pthread_mutex_destroy");
    }
}

void Mutex::lock(){
    if(pthread_mutex_lock(&_mutex)){
        perror("pthread_mutex_lock");
    }
}

void Mutex::unlock(){
    if(pthread_mutex_unlock(&_mutex)){
        perror("pthread_mutex_unlock");
    }
}

