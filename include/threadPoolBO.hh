#ifndef threadPool_hh_
#define threadPool_hh_

#include "threadBO.hh"
#include "taskQueBO.hh"
#include <vector>
#include <memory>

using std::vector;
using std::unique_ptr;
using std::bind;

using Task = function<void()>;

class ThreadPool{
    friend class WorkThread;
public:
    ThreadPool(size_t threadNum,size_t queSize);
    ~ThreadPool();
    void start();//启动线程池
    void stop();//结束线程池
    void addTask(Task&& pTask);//右值
    Task getTask();

private:
    void threadhandler();//交给子线程处理的任务

    size_t _threadNum;
    size_t _queSize;
    vector<unique_ptr<Thread>> _threads;//存放子线程容器
    TaskQueue _taskQue;
    bool _isExit;
};


#endif 