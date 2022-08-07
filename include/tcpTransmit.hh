//管理TCP传输的类
#ifndef tcpTransmit_hh_
#define tcpTransmit_hh_

#include "socket.hh"
#include "socketIO.hh"
#include "inetAddr.hh"
#include <memory>
#include <functional>

using std::shared_ptr;
using std::function;

class EventLoop;

class TcpTransmit
:public std::enable_shared_from_this<TcpTransmit>{
    using TcpTransmitPtr = shared_ptr<TcpTransmit>;
    using TcpTransmitCb = function<void(const TcpTransmitPtr &)>;
public:
    TcpTransmit(int fd, EventLoop* loop);
    ~TcpTransmit();
    void send(const string& msg);
    string recv();
    string recvn(int len);
    string toStr();
    bool isClosed() const;
    void setConnCb(const TcpTransmitCb &cb);
    void setMsgCb(const TcpTransmitCb &cb);
    void setCloseCb(const TcpTransmitCb &cb);
    void handleConnCb();
    void handleMsgCb();
    void handleCloseCb();
    void sendInLoop(const string& msg);
private:
    InetAddr getLocalAddr();
    InetAddr getPeerAddr();

    Socket _sock;
    SocketIO _sockIO;
    EventLoop* _loop;
    InetAddr _localAddr;
    InetAddr _peerAddr;
    TcpTransmitCb _onConnCb;
    TcpTransmitCb _onMsgCb;
    TcpTransmitCb _onCloseCb;
};

#endif 
