#include "../include/taskQueBO.hh"

TaskQueue::TaskQueue(size_t sz)
: _queSize(sz)
, _que()
, _mutex()
, _notEmpty(_mutex)
, _notFull(_mutex)
, _isExit(false){ }

TaskQueue::~TaskQueue(){ }

bool TaskQueue::empty() const{
    return 0 == _que.size();
}

bool TaskQueue::full() const{
    return _queSize == _que.size();
}

void TaskQueue::push(elemType&& value){
    //上锁(自动解锁) autolock栈对象
    MutexLockGuard autoLock(_mutex);
    //线程同步(根据队列满/空决定是否可以访问资源)
    //while防止虚假唤醒
    while(full()){
        _notFull.wait();
    }
    //入队
    _que.push(std::move(value));
    //唤醒阻塞在条件变量_notEmpty.wait()处的线程
    _notEmpty.notify();
}

elemType TaskQueue::pop(){
    //上锁
    MutexLockGuard autoLock(_mutex);
    //线程同步
    while(!_isExit && empty()){
        _notEmpty.wait();
    }    
    if(!_isExit){
        //取任务
        elemType tmp = _que.front();
        _que.pop();
        //唤醒阻塞在条件变量_notFull.wait()处的线程
        _notFull.notify();

        return tmp;
    }
    else{
        return nullptr;
    }
}   

void TaskQueue::wakeup(){
    //解决线程池无法退出bug
    _isExit = true;
    _notEmpty.notifyAll();
}