#include <iostream>
#include <system_error>
#include <filesystem>
#include <cstdlib>

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    // Windows: C:\Users\lelegard\Documents\demo-cpp\msbuild
    std::cout << "fs::current_path() = \"" << fs::current_path().string() << "\"" << std::endl;

    // Windows: C:\Users\lelegard\AppData\Local\Temp\ (eol)
    std::cout << "fs::temp_directory_path() = \"" << fs::temp_directory_path().string() << "\"" << std::endl;

    // Windows: C:\Users\lelegard\Documents\demo-cpp\non\existent
    std::cout << "fs::absolute(\"../non/existent\") = \"" << fs::absolute("../non/existent").string() << "\"" << std::endl;

    // Windows: C:\Users\lelegard\Documents\demo-cpp\non\existent
    std::cout << "fs::weakly_canonical(\"../non/existent\") = \"" << fs::weakly_canonical("../non/existent").string() << "\"" << std::endl;

    // Windows: C:\c\foo\top\non\existent
    std::cout << "fs::absolute(\"/c/foo/bar/../top/./non/existent\") = \"" << fs::absolute("/c/foo/bar/../top/./non/existent").string() << "\"" << std::endl;
}