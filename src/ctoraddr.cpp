#include <iostream>

class C
{
public:
    C() { std::cout << "ctor" << std::endl; }
    ~C() { std::cout << "dtor" << std::endl; }
    C& operator &() { return *this; }
};

void fref(C& c)
{
    std::cout << "fref" << std::endl;
}

int main(int argc, char* argv[])
{
    std::cout << "main (in)" << std::endl;
    fref(&C());
    std::cout << "main (out)" << std::endl;
}
