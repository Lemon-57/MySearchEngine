//建立TCP连接的类
#ifndef tcpConn_hh_
#define tcpConn_hh_

#include "socket.hh"
#include "inetAddr.hh"

class TcpConn{
public:
    TcpConn(const string& ip, unsigned short port);
    ~TcpConn();
    void prepareServ();
    int accept();
    int getListenFd();

private:
    void setReuseAddr();
    void setReusePort();
    void bind();
    void listen();

    Socket _listenSock;
    InetAddr _servAddr;
};



#endif 