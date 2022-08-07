#include "../include/eventLoop.hh"
#include <unistd.h>
#include <iostream>

using std::cout;
using std::endl;

EventLoop::EventLoop(TcpConn& conn)
: _epfd(createEpollFd())
, _evfd(createEvtFd())
, _conn(conn)
, _isLooping(false)
, _evtList(1024){
    //监听listenfd与eventfd
    //对新连接以及给客户端答复事件响应
    addEpollReadFd(_conn.getListenFd());
    addEpollReadFd(_evfd);
}

EventLoop::~EventLoop(){
    if(_epfd){
        close(_epfd);
    }
}

void EventLoop::loop(){
    _isLooping = true;
    while(_isLooping){
        epollWait();
    }
}

void EventLoop::unloop(){
    _isLooping = false;
}

void EventLoop::runInLoop(Functor&& cb){
    {
        //锁的粒度减小
        MutexLockGuard autoLock(_mutex);
        _pendingCb.push_back(std::move(cb));
    }

    //写eventfd唤醒卡在read处的
    writeEvtFd();
}

void EventLoop::setConnCb(TcpTransmitCb&& cb){
    _onConnCb = std::move(cb);
}

void EventLoop::setMsgCb(TcpTransmitCb&& cb){
    _onMsgCb = std::move(cb);
}

void EventLoop::setCloseCb(TcpTransmitCb&& cb){
    _onCloseCb = std::move(cb);
}

void EventLoop::epollWait(){
    int nready = -1;
    do{
        nready = epoll_wait(_epfd,
                            &*_evtList.begin(),
                            _evtList.size(),
                            3000);
    }while(-1 == nready && errno == EINTR);
    if(-1 == nready){
        perror("epoll_wait");
        return;
    }
    else if(0 == nready){
        cout << ">>epoll_wait timeout" << endl;
    }
    else{
        //监听链表扩容
        if(nready == static_cast<int>(_evtList.size())){
            _evtList.resize(2 * nready);
        }

        for(int idx = 0; idx < nready; ++idx){
            int fd = _evtList[idx].data.fd;
            //新连接
            if(fd == _conn.getListenFd()){
                //且是读事件
                if(_evtList[idx].events & EPOLLIN){
                    handleNewConn();
                }
            }
            //eventfd就绪
            else if(fd == _evfd){
                if(_evtList[idx].events & EPOLLIN){
                    readEvtFd();//读eventfd,从阻塞中醒来
                    handlePendingFunc();//处理事件(给客户端答复)
                }
            }
            //已连接的对端就绪
            else{
                //且是读事件
                if(_evtList[idx].events & EPOLLIN){
                    handleMsg(fd);//处理事件(接收客户端消息)
                }
            }
        }
    }
}

int EventLoop::createEpollFd(){
    int fd = epoll_create1(0);
    if(fd < 0){
        perror("epoll_create1");
    }
    return fd;
}

void EventLoop::addEpollReadFd(int fd){
    struct epoll_event evt;
    evt.data.fd = fd;
    evt.events = EPOLLIN;
    if(epoll_ctl(_epfd, 
                EPOLL_CTL_ADD,
                fd,
                &evt) < 0){
       perror("epoll_ctl");
       return;             
    }
}
void EventLoop::delEpollReadFd(int fd){
    struct epoll_event evt;
    evt.data.fd = fd;
    evt.events = EPOLLIN;
    if(epoll_ctl(_epfd, 
                EPOLL_CTL_DEL,
                fd,
                &evt) < 0){
       perror("epoll_ctl");
       return;             
    }
}

int EventLoop::createEvtFd(){
    int fd = eventfd(0, 0);
    if(-1 == fd){
        perror("eventfd");
        return -1;
    }
    return fd;
}

void EventLoop::writeEvtFd(){
    uint64_t counter = 1;
    int ret = write(_evfd, &counter, sizeof(counter));
    if(ret != sizeof(counter)){
        perror("write");
        return;
    }
}

void EventLoop::readEvtFd(){
    uint64_t counter = 0;
    int ret = read(_evfd, &counter, sizeof(counter));
    if(ret != sizeof(counter)){
        perror("write");
        return;
    }
}


void EventLoop::handlePendingFunc(){
    vector<Functor> tmp;//防止遍历事件过程中有新事件
    {
        MutexLockGuard autoLock(_mutex);
        tmp.swap(_pendingCb);
    }
    //处理事件
    for(auto& cb: tmp){
        cb();//调用回调函数
    }
}

void EventLoop::handleNewConn(){
    //建立连接
    int peerFd = _conn.accept();
    if(peerFd < 0){
        perror("accpet");
    }
    addEpollReadFd(peerFd);
    
    //准备传输工作
    TcpTransmitPtr transmit(new TcpTransmit(peerFd, this));
    //给transmit注册三个事件
    transmit->setConnCb(_onConnCb);
    transmit->setMsgCb(_onMsgCb);
    transmit->setCloseCb(_onCloseCb);
    //加入管理连接的map
    _conns.insert(std::make_pair(peerFd, transmit));
    //处理连接消息
    transmit->handleConnCb();
}

void EventLoop::handleMsg(int fd){
    map<int, TcpTransmitPtr>::iterator it = _conns.find(fd);
    if(it != _conns.end()){
        if(it->second->isClosed()){
            //处理连接断开
            it->second->handleCloseCb();
            delEpollReadFd(fd);
            _conns.erase(it);
        }
        else{
            //处理到达的消息
            it->second->handleMsgCb();
        }

    }else{
        cout << "connection doesn't exited." << endl;
    }
}

