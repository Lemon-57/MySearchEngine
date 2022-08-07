#ifndef noCopy_hh_
#define noCopy_hh_

//禁止复制控制语义
//protected构造析构--抽象类
class NoCopy{
protected:
    NoCopy(){ }
    ~NoCopy(){ }
    NoCopy(const NoCopy&) = delete;
    NoCopy& operator=(const NoCopy&) = delete;
};
#endif 