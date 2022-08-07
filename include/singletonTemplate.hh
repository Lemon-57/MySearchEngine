template <typename T>
class Singleton{
public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    
	template <typename... Args>
    static T* getInstance(Args... args){
        if(_pInstance == nullptr){
            _pInstance = new T(args...);
        }
        return _pInstance;
    }

private:
    Singleton(){}
    ~Singleton(){
        if(_pInstance){
                 delete _pInstance;
                _pInstance = nullptr;
        }
    }
    static T* _pInstance;
};

template <typename T>
T* Singleton<T>::_pInstance = nullptr;
