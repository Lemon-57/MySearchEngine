#include "../include/tcpServer.hh"

TcpServer::TcpServer(const string& ip, unsigned short port)
: _tcpConn(ip, port) 
, _loop(_tcpConn){ }

TcpServer::~TcpServer(){ }

void TcpServer::start(){
    _tcpConn.prepareServ();
    _loop.loop();
}

void TcpServer::stop(){
    _loop.unloop();
}

void TcpServer::setAllCallBack(TcpTransmitCb&& onConn,
                    TcpTransmitCb&& onMsg,
                    TcpTransmitCb&& onClose){
    _loop.setConnCb(std::move(onConn));
    _loop.setMsgCb(std::move(onMsg));
    _loop.setCloseCb(std::move(onClose));

}