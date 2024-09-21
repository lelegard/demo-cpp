#include <iostream>
#include <chrono>
#include <cstdlib>

using namespace std::chrono_literals;

template <class Rep, class Period>
void display(const char* name, std::chrono::duration<Rep,Period> d)
{
    std::cout << name << " = " << d.count() << " " << decltype(d)::period::num << "/" << decltype(d)::period::den << std::endl;
}

#define DISPLAY(e) display(#e, e)

int main(int argc, char* argv[])
{
    std::cout << "std::chrono::hours" << std::endl
              << "  sizeof: " << (8 * sizeof(std::chrono::hours)) << " bits" << std::endl
              << "  rep: " << (8 * sizeof(std::chrono::hours::rep)) << " bits" << std::endl
              << "  period::num = " << std::chrono::hours::period::num << std::endl
              << "  period::den = " << std::chrono::hours::period::den << std::endl
              << "std::chrono::seconds" << std::endl
              << "  sizeof: " << (8 * sizeof(std::chrono::seconds)) << " bits" << std::endl
              << "  rep: " << (8 * sizeof(std::chrono::seconds::rep)) << " bits" << std::endl
              << "  period::num = " << std::chrono::seconds::period::num << std::endl
              << "  period::den = " << std::chrono::seconds::period::den << std::endl
              << "std::chrono::milliseconds" << std::endl
              << "  sizeof: " << (8 * sizeof(std::chrono::milliseconds)) << " bits" << std::endl
              << "  rep: " << (8 * sizeof(std::chrono::milliseconds::rep)) << " bits" << std::endl
              << "  period::num = " << std::chrono::milliseconds::period::num << std::endl
              << "  period::den = " << std::chrono::milliseconds::period::den << std::endl;

    DISPLAY(2s);
    DISPLAY(20ms);
    DISPLAY(2s - 20ms);
}
