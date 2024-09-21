#pragma once
#include "atexit.h"
#include <mutex>

namespace ts {

    // A generic allocator. Using specialization on TAG, a non-default constructor can be called.
    template <typename T, typename TAG = void>
    T* Allocate()
    {
        return new T;
    }

    // A template class to define a singleton.
    // The class to "singletonize" must have a public default constructor.
    template <typename T, typename TAG = void>
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


template <typename T, typename TAG>
T* ts::Singleton<T,TAG>::_getInstance()
{
    // Use a double check lock to avoid calling call_once() more than necessary.
    // To avoid a race condition, _instance must be volatile.
    if (_instance == nullptr) {
        std::call_once(_once_flag, [this]() {
            this->_instance = Allocate<T,TAG>();
            ts::atexit(_cleanup, this);
        });
    }
    return _instance;
}

template <typename T, typename TAG>
void ts::Singleton<T,TAG>::_cleanup(void* param)
{
    typename ts::Singleton<T,TAG>* sgton = reinterpret_cast<typename ts::Singleton<T,TAG>*>(param);
    if (sgton != nullptr) {
        delete sgton->_instance;
        sgton->_instance = nullptr;
    }
}
