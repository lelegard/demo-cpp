#include <iostream>
#include <vector>

using block = std::vector<int>;

void display(const char* name, const block& b)
{
    std::cout << name << "(" << b.size() << ") = {";
    for (auto i : b) {
        std::cout << i << ", ";
    }
    std::cout << "}" << std::endl;
}

class A
{
public:
    block b{};
    A(int s, int v) : b(s, v) {}
};

class C
{
public:
    A a{3, 8};
    block b1{3, 8};
    block b2{block(3, 8)};
    C() = default;
    C(int) : b1(3,8) {}
};

int main(int argc, char* argv[])
{
    block b1;
    block b2{4, 5};
    block b3(3, 27);
    C c1;
    C c2(0);

    display("b1", b1);
    display("b2", b2);
    display("b3", b3);
    display("c1.a.b", c1.a.b);
    display("c1.b1", c1.b1);
    display("c1.b2", c1.b2);
    display("c2.a.b", c2.a.b);
    display("c2.b1", c2.b1);
    display("c2.b2", c2.b2);
}
