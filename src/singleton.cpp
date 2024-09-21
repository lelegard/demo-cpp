#include <iostream>
#include <vector>
#include <mutex>

//-----------------------------------------------------------------------------
// A re-implementation of std::atexit() with:
// - Unlimited functions (std::atexit() can only guarantee 32 entries)
// - Pass a parameter to the function.
//-----------------------------------------------------------------------------

namespace ts {
    void atexit(void (*func)(void*), void* param = nullptr);
}

namespace {

    class ExitContext
    {
    public:
        static ExitContext& Instance();
        void add(void (*func)(void*), void* param);
    private:
        using ExitHandler = std::pair<void (*)(void*), void*>;

        ExitContext() = default;
        std::mutex _mutex {};
        std::vector<ExitHandler> _handlers {};

        static ExitContext* volatile _instance;
        static std::once_flag _once_flag;
        static void _cleanup();
    };

    ExitContext* volatile ExitContext::_instance = nullptr;
    std::once_flag ExitContext::_once_flag {};

    ExitContext& ExitContext::Instance()
    {
        // Use a double check lock to avoid calling call_once() more than necessary.
        // To avoid a race condition, _instance must be volatile.
        if (_instance == nullptr) {
            std::call_once(_once_flag, []() {
                _instance = new ExitContext;
                // std::atexit() is called only once, well below the 32 entries limit.
                std::atexit(ExitContext::_cleanup);
            });
        }
        return *_instance;    
    }

    void ExitContext::add(void (*func)(void*), void* param)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _handlers.push_back(std::make_pair(func, param));
    }

    void ExitContext::_cleanup()
    {
        if (_instance != nullptr) {
            // No need to lock, we are in a std::atexit() handler, during process termination.
            for (auto it = _instance->_handlers.rbegin(); it != _instance->_handlers.rend(); ++it) {
                it->first(it->second);
            }
            delete _instance;
            _instance = nullptr;
        }
    }
}

void ts::atexit(void (*func)(void*), void* param)
{
    ExitContext::Instance().add(func, param);
}

//-----------------------------------------------------------------------------
// A template class to define a singleton.
// The class to "singletonize" must have a public default constructor.
//-----------------------------------------------------------------------------

namespace ts {

    template <typename T>
    class Singleton
    {
    public:
        // The default constructor is constexpr, ensuring that static Singleton objects are statically initialized.
        constexpr Singleton() = default;
        T* operator->() { return _getInstance(); }
        T& operator*() { return *_getInstance(); }
    private:
        T* _getInstance();
        T* volatile _instance = nullptr;
        std::once_flag _once_flag {};
        static void _cleanup(void*);
    };
}

template <typename T>
T* ts::Singleton<T>::_getInstance()
{
    // Use a double check lock to avoid calling call_once() more than necessary.
    // To avoid a race condition, _instance must be volatile.
    if (_instance == nullptr) {
        std::call_once(_once_flag, [this]() {
            this->_instance = new T;
            ts::atexit(_cleanup, this);
        });
    }
    return _instance;
}

template <typename T>
void ts::Singleton<T>::_cleanup(void* param)
{
    typename ts::Singleton<T>* sgton = reinterpret_cast<typename ts::Singleton<T>*>(param);
    if (sgton != nullptr) {
        delete sgton->_instance;
        sgton->_instance = nullptr;
    }
}


//-----------------------------------------------------------------------------
// A test class.
//-----------------------------------------------------------------------------

class Obj
{
private:
    std::ostream& at() const { std::cout << "Obj@" << std::hex << size_t(this) << std::dec; return std::cout; }
public:
    Obj() { at() << ": constructor" << std::endl; }
    Obj(int i, const std::string& s) { at() << ": constructor (" << i << ", \"" << s << "\")" << std::endl; }
    ~Obj() { at() << ": destructor" << std::endl; }
    void use() const { at() << ": use" << std::endl; }
};

//-----------------------------------------------------------------------------

extern ts::Singleton<Obj> a;
extern ts::Singleton<Obj> b;

ts::Singleton<Obj> a;
ts::Singleton<Obj> b;

namespace {
    void ender(void* param)
    {
        const char* str = reinterpret_cast<const char*>(param);
        std::cout << "ender: \"" << str << "\"" << std::endl;
    }
}

int main(int argc, char* argv[])
{
    std::cout << "main: begin" << std::endl;
    Obj aa;
    Obj bb(1, "foo");
    ts::atexit(ender, const_cast<char*>("aaaa"));
    ts::atexit(ender, const_cast<char*>("bbbb"));
    std::cout << "main: accessing singletons" << std::endl;
    a->use();
    b->use();
    std::cout << "main: end" << std::endl;
}
