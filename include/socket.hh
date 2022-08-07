//管理socket套接字的类(稀缺资源，RAII)
#ifndef socket_hh_
#define socket_hh_

#include "noCopy.hh"


class Socket: public NoCopy{
public:
    Socket();
    explicit Socket(int fd);
    ~Socket();
    int getFd() const;
    void shutdownWr();
private:
    int _fd;
};

#endif 