#include <iostream>
#include <system_error>

class C: public std::error_code
{
public:
    C() { std::cout << "C()" << std::endl; }
    C(int i) { std::cout << "C(" << i << ")" << std::endl; }
    ~C() { std::cout << "~C()" << std::endl; }
    C& operator &() { return *this; }
};

void fref(std::error_code& a)
{
    std::cout << "fref()" << std::endl;
}

int main(int argc, char* argv[])
{
    std::cout << "main - 1" << std::endl;
    fref(&C());
    std::cout << "main - 2" << std::endl;
    fref(&C(1));
    std::cout << "main - 3" << std::endl;
}
