#pragma once

namespace ts {

    // A re-implementation of std::atexit() with:
    // - Unlimited functions (std::atexit() can only guarantee 32 entries)
    // - Pass a parameter to the function.

    void atexit(void (*func)(void*), void* param = nullptr);
}
