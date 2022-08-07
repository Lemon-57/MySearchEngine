#include "../include/tcpTransmit.hh"
#include "../include/eventLoop.hh"
#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::ostringstream;

TcpTransmit::TcpTransmit(int fd, EventLoop* loop)
: _loop(loop)
, _sock(fd)
, _sockIO(fd)
, _localAddr(getLocalAddr())
, _peerAddr(getPeerAddr()){ }

TcpTransmit::~TcpTransmit(){ }

void TcpTransmit::send(const string& msg){
    _sockIO.writen(msg.c_str(), msg.size());
}

string TcpTransmit::recv(){
    char buf[65535] = {0};
    _sockIO.readline(buf, 65535);

    return string(buf);
}

string TcpTransmit::recvn(int len){
    string res;
    int left = len;
    while(left > 0){
    char buf[65535] = {0};
    int ret = _sockIO.readn(buf, left);
    res += buf;
    left -= ret;
    }
    return res;
}


string TcpTransmit::toStr(){
    ostringstream oss;
    oss << _localAddr.ip() << ":"
        << _localAddr.port() << "---->"
        << _peerAddr.ip() << ":"
        << _peerAddr.port();

    return oss.str();
}

InetAddr TcpTransmit::getLocalAddr(){
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr );
    if(-1 == getsockname(_sock.getFd(), (struct sockaddr *)&addr, &len)){
        perror("getsockname");
    }

    return InetAddr(addr);
}

InetAddr TcpTransmit::getPeerAddr(){
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr );
    if(-1 == getpeername(_sock.getFd(), (struct sockaddr *)&addr, &len)){
        perror("getpeername");
    }

    return InetAddr(addr);
}

bool TcpTransmit::isClosed() const{
    char buf[1] = {0};
    //MSG_PEEK
    int ret = ::recv(_sock.getFd(), buf, sizeof(buf), MSG_PEEK);
    return ret == 0;
}

void TcpTransmit::setConnCb(const TcpTransmitCb &cb){
    _onConnCb = cb;
}

void TcpTransmit::setMsgCb(const TcpTransmitCb &cb){
    _onMsgCb = cb;
}

void TcpTransmit::setCloseCb(const TcpTransmitCb &cb){
    _onCloseCb = cb;
}

void TcpTransmit::handleConnCb(){
    //防止智能指针误用(两不同智能指针托管同一裸指针)
    if(_onConnCb){
        _onConnCb(shared_from_this());
    }
}

void TcpTransmit::handleMsgCb(){
    if(_onMsgCb){
        _onMsgCb(shared_from_this());
    }
}

void TcpTransmit::handleCloseCb(){
    if(_onCloseCb){
        _onCloseCb(shared_from_this());
    }
}

void TcpTransmit::sendInLoop(const string& msg){
    //消息msg要从TcpTransmit发送给EventLoop
    if(_loop){
        _loop->runInLoop(std::bind(&TcpTransmit::send, this, msg));
    }
}
