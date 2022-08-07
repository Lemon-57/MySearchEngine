//基于对象的线程封装
#ifndef threadBO_hh_
#define threadBO_hh_

#include <pthread.h>
#include <functional>

using std::function;

using ThreadCallback = function<void()>;

class Thread{
public:
    Thread(ThreadCallback &&cb);//注册回调函数
    ~Thread();
    void start();
    void join();
private:
    static void *threadFunc(void *arg);

    pthread_t _tid;
    bool _isRunning;
    ThreadCallback _cb;//回调函数
};

#endif
