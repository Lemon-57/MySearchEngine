#ifndef condition_hh_
#define condition_hh_

#include "noCopy.hh"
#include <pthread.h>

class Mutex;

class Condition: public NoCopy{
public:
    Condition(Mutex& mutex);
    ~Condition();
    void wait();
    void notify();
    void notifyAll();
private:
    Mutex& _mutex;
    pthread_cond_t _cond;
};

#endif