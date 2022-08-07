#ifndef taskQue_hh_
#define taskQue_hh_

#include "mutex.hh"
#include "condition.hh"
#include <queue>
#include <functional>

using std::queue;
using std::function;

using elemType = function<void()>;

class TaskQueue{
public:
    TaskQueue(size_t sz);
    ~TaskQueue();
    bool empty() const;
    bool full() const;
    void push(elemType&& value);
    elemType pop();
    void wakeup();
private:
    size_t _queSize;
    queue<elemType> _que;
    Mutex _mutex;
    Condition _notEmpty;
    Condition _notFull;
    bool _isExit;
};

#endif 