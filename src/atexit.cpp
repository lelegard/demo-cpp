#include "atexit.h"
#include <cstdlib>
#include <vector>
#include <mutex>

//-----------------------------------------------------------------------------
// Hand-crafted singleton for ts::atexit().
//-----------------------------------------------------------------------------

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
