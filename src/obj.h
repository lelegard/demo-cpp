#pragma once
#include "singleton.h"
#include <iostream>

// A test class.
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

// An allocator with a special tag.
enum Foo {A, B, C};

namespace ts {
    template<> Obj* Allocate<Obj,Foo>();
}

extern ts::Singleton<Obj> a;
extern ts::Singleton<Obj,Foo> b;
