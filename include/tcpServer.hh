#ifndef tcpServer_hh_
#define tcpServer_hh_

#include "eventLoop.hh"

class TcpServer{
public:
    TcpServer(const string& ip, unsigned short port);
    ~TcpServer();
    void start();
    void stop();
    void setAllCallBack(TcpTransmitCb&& onConn,
                        TcpTransmitCb&& onMsg,
                        TcpTransmitCb&& onClose);

private:
    TcpConn _tcpConn;
    EventLoop _loop;
    
};

#endif 