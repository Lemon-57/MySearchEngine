#ifndef MYLOG_HH_
#define MYLOG_HH_

#include <log4cpp/Category.hh>
#include <string>

using std::string;
using namespace log4cpp;

#define prefix(msg) string("[")\
	.append(__FILE__).append(":")\
	.append(__FUNCTION__).append(":")\
	.append(std::to_string(__LINE__)).append("] ")\
	.append(msg).c_str()


//##__VA_ARGS__ 宏前面加上##的作用在于，当可变参数的个数为0时，
//这里的##起到把前面多余的","去掉的作用,否则会编译出错
#define LogError(msg,...) MyLog::getLogInstance()->error(prefix(msg), ##__VA_ARGS__)
#define LogWarn(msg,...) MyLog::getLogInstance()->warn(prefix(msg), ##__VA_ARGS__)
#define LogInfo(msg,...) MyLog::getLogInstance()->info(prefix(msg), ##__VA_ARGS__)
#define LogDebug(msg,...) MyLog::getLogInstance()->debug(prefix(msg), ##__VA_ARGS__)

class MyLog
{
public:
    
    MyLog(const MyLog &) = delete;
    MyLog & operator=(const MyLog &) = delete;

    static MyLog * getLogInstance();

    class AutoRelease{
    public:
        AutoRelease(){}
        ~AutoRelease();
    };

    template <typename... Args>
	void error(const char* msg, Args... args){
        _log.error(msg, args...);
    }

    template <typename ...Args>
    void warn(const char* msg, Args... args){
        _log.warn(msg, args...);
    }
    
    template <typename ...Args>
    void info(const char* msg, Args... args){
        _log.info(msg, args...);
    }

    template <typename ...Args>
    void debug(const char* msg, Args... args){
        _log.debug(msg, args...);
    }

    void warn(const char * msg);
	void error(const char * msg);
	void info(const char * msg);
	void debug(const char * msg);
    
private:
    MyLog();
    ~MyLog(){}
    static MyLog* _pLogInstance;
    static AutoRelease _auto;
    Category & _log;
};

#endif
