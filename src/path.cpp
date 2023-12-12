#include <iostream>
#include <system_error>
#include <filesystem>
#include <cstdlib>

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    fs::path p;
    std::error_code err;

    // Linux: 
    // Windows: 2
    std::cout << "sizeof(fs::path::value_type) = " << sizeof(fs::path::value_type) << std::endl;

    // Linux: 
    // Windows: "foo/bar\boo.foo"
    p = "foo/bar";
    std::cout << "\"foo/bar\" /= \"boo.foo\" = \"" << (p /= "boo.foo").string() << "\"" << std::endl;

    // Linux: 
    // Windows: "foo/bar/boo.foo"
    p = "foo/bar/";
    std::cout << "\"foo/bar/\" /= \"boo.foo\" = \"" << (p /= "boo.foo").string() << "\"" << std::endl;

    // Linux: 
    // Windows: "foo/barboo.foo"
    p = "foo/bar";
    std::cout << "\"foo/bar\" += \"boo.foo\" = \"" << (p += "boo.foo").string() << "\"" << std::endl;

    // Linux: 
    // Windows: "foo/bar/boo.foo"
    p = "foo/bar/";
    std::cout << "\"foo/bar/\" += \"boo.foo\" = \"" << (p += "boo.foo").string() << "\"" << std::endl;

    // Linux: 
    // Windows: "foo\..\.\bar"
    p = "foo/../.\\bar";
    std::cout << "\"foo/../.\\bar\".make_preferred() = \"" << p.make_preferred().string() << "\"" << std::endl;

    // Linux: 
    // Windows: "C:\Users\lelegard\Documents\demo-cpp\msbuild"
    std::cout << "fs::current_path() = \"" << fs::current_path().string() << "\"" << std::endl;

    // Linux: 
    // Windows: "C:\Users\lelegard\AppData\Local\Temp\"
    std::cout << "fs::temp_directory_path() = \"" << fs::temp_directory_path().string() << "\"" << std::endl;

    // Linux: 
    // Windows: "C:\Users\lelegard\Documents\demo-cpp\non\existent"
    std::cout << "fs::absolute(\"../non/existent\") = \"" << fs::absolute("../non/existent").string() << "\"" << std::endl;

    // Linux: 
    // Windows: "C:\Users\lelegard\Documents\demo-cpp\non\existent"
    std::cout << "fs::weakly_canonical(\"../non/existent\") = \"" << fs::weakly_canonical("../non/existent").string() << "\"" << std::endl;

    // Linux: 
    // Windows: "C:\c\foo\top\non\existent"
    std::cout << "fs::absolute(\"/c/foo/bar/../top/./non/existent\") = \"" << fs::absolute("/c/foo/bar/../top/./non/existent").string() << "\"" << std::endl;

    // Linux: 
    // Windows: "\c\foo\top\non\existent"
    std::cout << "fs::weakly_canonical(\"/c/foo/bar/../top/./non/existent\") = \"" << fs::weakly_canonical("/c/foo/bar/../top/./non/existent").string() << "\"" << std::endl;

    // Linux: 
    // Windows: "C:\Users\lelegard\Documents\demo-cpp\top\non\existent"
    std::cout << "fs::weakly_canonical(\"../top/./non/existent\") = \"" << fs::weakly_canonical("../top/./non/existent").string() << "\"" << std::endl;

    // Linux: 
    // Windows: "..\bar\baz"
    std::cout << "fs::relative(\"foo/bar/baz\", \"foo/com\") = \"" << fs::relative("foo/bar/baz", "foo/com").string() << "\"" << std::endl;

    // Linux: 
    // Windows: "..\seg\media.ts"
    std::cout << "fs::relative(\"http://host/dir/seg/media.ts\", \"http://host/dir/list.txt\") = \"" << fs::relative("http://host/dir/seg/media.ts", "http://host/dir/list.txt").string() << "\"" << std::endl;

    // Linux: 
    // Windows: "http://host/dir/"
    std::cout << "\"http://host/dir/list.txt\".remove_filename() = \"" << fs::path("http://host/dir/list.txt").remove_filename().string() << "\"" << std::endl;
}
