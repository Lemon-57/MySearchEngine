//事件循环类
#ifndef eventLoop_hh_
#define eventLoop_hh_

#include "tcpConn.hh"
#include "tcpTransmit.hh"
#include "mutex.hh"
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <map>
#include <memory>
#include <functional>
#include <vector>

using std::shared_ptr;
using std::function;
using std::vector;
using std::map;

using Functor = function<void()>;
using TcpTransmitPtr = shared_ptr<TcpTransmit>;
using TcpTransmitCb = function<void(const TcpTransmitPtr &)>;

class EventLoop{
public:
    EventLoop(TcpConn& conn);
    ~EventLoop();
    void loop();
    void unloop();
    void runInLoop(Functor&& cb);
    void setConnCb(TcpTransmitCb&& cb);
    void setMsgCb(TcpTransmitCb&& cb);
    void setCloseCb(TcpTransmitCb&& cb);
private:
    int createEvtFd();
    int createEpollFd();
    void epollWait();
    void addEpollReadFd(int fd);
    void delEpollReadFd(int fd);
    void readEvtFd();
    void writeEvtFd();
    void handleNewConn();
    void handleMsg(int fd);
    void handlePendingFunc();

    int _epfd;
    int _evfd;
    Mutex _mutex;
    TcpConn& _conn;
    bool _isLooping;
    vector<Functor> _pendingCb;
    TcpTransmitCb _onConnCb;
    TcpTransmitCb _onMsgCb;
    TcpTransmitCb _onCloseCb;
    map<int, TcpTransmitPtr> _conns;
    vector<struct epoll_event> _evtList;
};

#endif 