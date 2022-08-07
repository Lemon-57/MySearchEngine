#include "../include/myLog.hh"
#include <iostream>
#include <log4cpp/Priority.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/RollingFileAppender.hh>

using std::cout;

MyLog * MyLog::_pLogInstance = nullptr;
MyLog::AutoRelease _auto;

MyLog * MyLog::getLogInstance(){
    if(nullptr == _pLogInstance){
        _pLogInstance = new MyLog();
    }
    return _pLogInstance;
}

MyLog::MyLog()
: _log(Category::getRoot().getInstance("LOG")){
    //1.设置布局
    auto ptnLayout = new PatternLayout();
    ptnLayout->setConversionPattern("%d [%c] [%p] %m%n");

    //不同的appender要用不同的layout
    auto ptnLayout2 = new PatternLayout();
    ptnLayout2->setConversionPattern("%d [%c] [%p] %m%n");
    
    //2. 设置日志的目的地
    auto posAppender = new OstreamAppender("console", &cout);
    posAppender->setLayout(ptnLayout);

    auto pRollingFileAppender = new RollingFileAppender(
            "rollingfileApp", 
            "../log/MyLogRolling.log",
            1024*10, 3);
    pRollingFileAppender->setLayout(ptnLayout2);
    
    //3.优先级与目的地
    _log.setPriority(Priority::DEBUG);
    _log.setAppender(posAppender);
    _log.addAppender(pRollingFileAppender);
}

void MyLog::warn(const char *msg)
{
	_log.warn(msg);
}

void MyLog::error(const char *msg)
{
	_log.error(msg);
}

void MyLog::info(const char *msg)
{
	_log.info(msg);
}

void MyLog::debug(const char *msg)
{
	_log.debug(msg);
}

MyLog::AutoRelease::~AutoRelease(){
    if(_pLogInstance){
        //手动释放这些对象，则会造成内存报错
        //Log4cpp使用了一个内部类来管理这些对象。
        //此类的名称是HierarchyMaintainer，它负责管理Category的继承关系，
        //在程序结束时，HierarchyMaintainer会依次释放所有Category，
        //而Category则会依次释放拥有的有效Appender，Appender则会释放所有附属的Layout。
        //Category::shutdown();

        delete _pLogInstance;
       _pLogInstance = nullptr;
  
    }
}

