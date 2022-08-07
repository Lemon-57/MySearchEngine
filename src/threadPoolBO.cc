#include "../include/threadPoolBO.hh"
#include <unistd.h>

ThreadPool::ThreadPool(size_t threadNum,size_t queSize)
: _threadNum(threadNum)
, _queSize(queSize)
, _taskQue(_queSize)
, _isExit(false){ 
    _threads.reserve(_threadNum);//预留空间
}

ThreadPool::~ThreadPool(){
    if(!_isExit){
        stop();
        _isExit = true;
    }
}

void ThreadPool::start(){
    //创建工作线程并存到容器中
    for(int i = 0; i < _threadNum; i++){
        unique_ptr<Thread> up(new Thread(bind(&ThreadPool::threadhandler, this)));
        _threads.push_back(std::move(up));//unique_ptr可以作为容器元素，要用右值
    }

    //运行工作线程
    for(auto& t : _threads){
        t->start();
    }
}

void ThreadPool::stop(){
    //bug1:任务队列中还有任务，但线程池已结束
    //原因:addTask结束后在stop中直接将_isExit置为true
    //解决方案:任务队列中只要还有数据，就要执行完毕，不能让子线程退出
    while(!_taskQue.empty()){
        sleep(1);
    }

    _isExit = true;
    //bug2:线程池工作结束无法正常退出
    //原因:执行快的线程在任务队列空但_isExit还没置为true时，
    //又进入while(!_isExit)循环->getTask->pop->wait处阻塞
    //解决方案:退出时唤醒所有线程
    _taskQue.wakeup();

    for(auto& t : _threads){
        t->join();
    }
}
void ThreadPool::addTask(Task&& pTask){
    if(pTask){
        _taskQue.push(std::move(pTask));
    }
}

Task ThreadPool::getTask(){
    return _taskQue.pop();
}

void ThreadPool::threadhandler(){
    while(!_isExit){
        Task tcb = getTask();
        if(tcb){
            tcb();//执行具体任务
        }
    }
}
