//网络地址类
#ifndef inetAddr_hh_
#define inetAddr_hh_

#include <arpa/inet.h>
#include <string>

using std::string;

class InetAddr{
public:
    InetAddr(const string& ip, unsigned short port);
    InetAddr(const struct sockaddr_in& addr);
    ~InetAddr();
    string ip() const;
    unsigned short port() const;
    const struct sockaddr_in* getInetAddr() const;

private:
    struct sockaddr_in _addr;
};


#endif 