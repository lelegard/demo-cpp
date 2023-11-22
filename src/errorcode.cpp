#include <iostream>
#include <system_error>
#include <filesystem>
#include <cstdlib>
#include <cerrno>

#if defined(_MSC_VER)
#include <WinSock2.h>
#include <Windows.h>
#endif

void display(const std::error_code& err, const std::string& title)
{
    std::cout << "=== " << title << std::endl
              << "    bool(err) = " << bool(err) << std::endl
              << "    err.value() = " << err.value() << std::endl
              << "    err.message() = \"" << err.message() << "\"" << std::endl
              << "    err.category().name() = \"" << err.category().name() << "\"" << std::endl;
}

int main(int argc, char* argv[])
{
    {
        std::error_code err;
        display(err, "std::error_code err");
    }
    {
        std::error_code err(EPERM, std::generic_category());
        display(err, "std::error_code err(EPERM, std::generic_category())");
    }
    {
        // Does not work on Windows, system_category() applies to Windows error code.
        std::error_code err(EPERM, std::system_category());
        display(err, "std::error_code err(EPERM, std::system_category())");
    }
#if defined(__linux__)
    {
        std::error_code err(EDQUOT, std::generic_category());
        display(err, "std::error_code err(EDQUOT, std::generic_category())");
    }
    {
        std::error_code err(EDQUOT, std::system_category());
        display(err, "std::error_code err(EDQUOT, std::system_category())");
    }
#elif defined(_MSC_VER)
    {
        // Windows error code
        std::error_code err(ERROR_ACCESS_DENIED, std::system_category());
        display(err, "std::error_code err(ERROR_ACCESS_DENIED, std::system_category())");
    }
    {
        // Windows error code
        std::error_code err(WSA_NOT_ENOUGH_MEMORY, std::system_category());
        display(err, "std::error_code err(WSA_NOT_ENOUGH_MEMORY, std::system_category())");
    }
    {
        // Windows error code
        std::error_code err(WSAEADDRINUSE, std::system_category());
        display(err, "std::error_code err(WSAEADDRINUSE, std::system_category())");
    }
#endif
}
