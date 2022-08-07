#include "../include/tcpConn.hh"
#include <stdio.h>

TcpConn::TcpConn(const string& ip, unsigned short port)
: _listenSock()
, _servAddr(ip, port){ }

TcpConn::~TcpConn(){ }

void TcpConn::prepareServ(){
    setReuseAddr();
    setReusePort();
    bind();
    listen();
}

int TcpConn::accept(){
    int connFd = ::accept(_listenSock.getFd(), nullptr, nullptr);
    if(-1 == connFd){
        perror("accept");
        return -1;
    }
    return connFd;
}

int TcpConn::getListenFd(){
    return _listenSock.getFd();
}

void TcpConn::setReuseAddr(){
    int opt = 1;
    if(-1 == setsockopt(_listenSock.getFd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))){
        perror("setsockopt");
        return;
    }
}

void TcpConn::setReusePort(){
    int opt = 1;
    if(-1 == setsockopt(_listenSock.getFd(), SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))){
        perror("setsockopt");
        return;
    }
}

void TcpConn::bind(){
    if(-1 == ::bind(_listenSock.getFd(), 
                    (struct sockaddr*)_servAddr.getInetAddr(),
                    sizeof(struct sockaddr))){
        perror("bind");
        return;
    }
}

void TcpConn::listen(){
    if(-1 == ::listen(_listenSock.getFd(), 128)){
        perror("listen");
        return;
    }
}


