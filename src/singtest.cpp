#include <iostream>
#include "atexit.h"
#include "obj.h"

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
