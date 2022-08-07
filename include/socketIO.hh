//真正做IO操作的类
#ifndef socketIO_hh_
#define socketIO_hh_

class SocketIO{
public:
    explicit SocketIO(int fd);
    ~SocketIO();
    int readn(char* buf, int len);
    int readline(char* buf, int len);
    int writen(const char* buf, int len);

private:
    int _fd;
};

#endif 