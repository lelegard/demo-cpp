#pragma once
#include "atexit.h"
#include <mutex>

namespace ts {
    //!
    //! Template allocator function with possible specialization.
    //! @tparam T The type of the data to allocate.
    //! @tparam TAG A tag type which is used to define template specializations.
    //! Using specialization on @a TAG, a non-default constructor can be called.
    //! The @a TAG type is only used to discriminate among various specialization;
    //! it is not used otherwise.
    //!
    template <typename T, typename TAG = void>
    T* Allocate()
    {
        return new T;
    }

    //!
    //! A template class to hold a singleton.
    //!
    //! The Singleton instance holds a pointer to the unique @a T instance, nothing else.
    //! The @a T instance is created the first time it is used and automatically deleted
    //! when the application terminates. All singleton are deleted in reverse order of
    //! their creation.
    //! @tparam T The type of the singleton data.
    //! @tparam TAG A tag type which is used to define template specializations.
    //! Using specialization on @a TAG, a non-default constructor can be called.
    //! @see Allocate
    //!
    template <typename T, typename TAG = void>
    class Singleton
    {
        // TS_NOCOPY(Singleton);
    public:
        //!
        //! Default constructor.
        //! The default constructor is constexpr, ensuring that static Singleton objects are statically initialized.
        //!
        constexpr Singleton() = default;

        //!
        //! Access the singleton data.
        //! The singleton is created if it does not exist yet.
        //! @return The address of the singleton data.
        //!
        T* operator->() { return _getInstance(); }

        //!
        //! Access the singleton data.
        //! The singleton is created if it does not exist yet.
        //! @return A reference to the singleton data.
        //!
        T& operator*() { return *_getInstance(); }

    private:
        T* volatile _instance = nullptr;
        std::once_flag _once_flag {};

        // Get / create the singleton instance (thread-safe).
        T* _getInstance();

        // Called when the application terminates.
        static void _cleanup(void*);
    };
}


//----------------------------------------------------------------------------
// Template definitions.
//----------------------------------------------------------------------------

// Get / create the singleton instance (thread-safe).
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

// Called when the application terminates.
template <typename T, typename TAG>
void ts::Singleton<T,TAG>::_cleanup(void* param)
{
    ts::Singleton<T,TAG>* sgton = reinterpret_cast<ts::Singleton<T,TAG>*>(param);
    if (sgton != nullptr) {
        delete sgton->_instance;
        sgton->_instance = nullptr;
    }
}
