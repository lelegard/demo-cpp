#include <iostream>
#include <system_error>
#include <filesystem>
#include <cstdlib>

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    fs::path p;
    std::error_code err;

    // Linux:   1
    // Windows: 2
    std::cout << "sizeof(fs::path::value_type) = " << sizeof(fs::path::value_type) << std::endl;

    // Linux:   "foo/bar/boo.foo"
    // Windows: "foo/bar\boo.foo"
    p = "foo/bar";
    std::cout << "\"foo/bar\" /= \"boo.foo\" = \"" << (p /= "boo.foo").string() << "\"" << std::endl;

    // Linux:   "foo/bar/boo.foo"
    // Windows: "foo/bar/boo.foo"
    p = "foo/bar/";
    std::cout << "\"foo/bar/\" /= \"boo.foo\" = \"" << (p /= "boo.foo").string() << "\"" << std::endl;

    // Linux:   "foo/barboo.foo"
    // Windows: "foo/barboo.foo"
    p = "foo/bar";
    std::cout << "\"foo/bar\" += \"boo.foo\" = \"" << (p += "boo.foo").string() << "\"" << std::endl;

    // Linux:   "foo/bar/boo.foo"
    // Windows: "foo/bar/boo.foo"
    p = "foo/bar/";
    std::cout << "\"foo/bar/\" += \"boo.foo\" = \"" << (p += "boo.foo").string() << "\"" << std::endl;

    // Linux:   "foo/../.\bar"
    // Windows: "foo\..\.\bar"
    p = "foo/../.\\bar";
    std::cout << "\"foo/../.\\bar\".make_preferred() = \"" << p.make_preferred().string() << "\"" << std::endl;

    // Linux:   "/home/lelegard/demo-cpp"
    // Windows: "C:\Users\lelegard\Documents\demo-cpp\msbuild"
    std::cout << "fs::current_path() = \"" << fs::current_path().string() << "\"" << std::endl;

    // Linux:   "/tmp"
    // Windows: "C:\Users\lelegard\AppData\Local\Temp\"
    std::cout << "fs::temp_directory_path() = \"" << fs::temp_directory_path().string() << "\"" << std::endl;

    // Linux:   "/home/lelegard/demo-cpp/../non/existent"
    // Windows: "C:\Users\lelegard\Documents\demo-cpp\non\existent"
    std::cout << "fs::absolute(\"../non/existent\") = \"" << fs::absolute("../non/existent").string() << "\"" << std::endl;

    // Linux:   "/home/lelegard/non/existent"
    // Windows: "C:\Users\lelegard\Documents\demo-cpp\non\existent"
    std::cout << "fs::weakly_canonical(\"../non/existent\") = \"" << fs::weakly_canonical("../non/existent").string() << "\"" << std::endl;

    // Linux:   "/c/foo/bar/../top/./non/existent"
    // Windows: "C:\c\foo\top\non\existent"
    std::cout << "fs::absolute(\"/c/foo/bar/../top/./non/existent\") = \"" << fs::absolute("/c/foo/bar/../top/./non/existent").string() << "\"" << std::endl;

    // Linux:   "/c/foo/top/non/existent"
    // Windows: "\c\foo\top\non\existent"
    std::cout << "fs::weakly_canonical(\"/c/foo/bar/../top/./non/existent\") = \"" << fs::weakly_canonical("/c/foo/bar/../top/./non/existent").string() << "\"" << std::endl;

    // Linux:   "/home/lelegard/top/non/existent"
    // Windows: "C:\Users\lelegard\Documents\demo-cpp\top\non\existent"
    std::cout << "fs::weakly_canonical(\"../top/./non/existent\") = \"" << fs::weakly_canonical("../top/./non/existent").string() << "\"" << std::endl;

    // Linux:   "../bar/baz"
    // Windows: "..\bar\baz"
    std::cout << "fs::relative(\"foo/bar/baz\", \"foo/com\") = \"" << fs::relative("foo/bar/baz", "foo/com").string() << "\"" << std::endl;

    // Linux:   "../seg/media.ts"
    // Windows: "..\seg\media.ts"
    std::cout << "fs::relative(\"http://host/dir/seg/media.ts\", \"http://host/dir/list.txt\") = \"" << fs::relative("http://host/dir/seg/media.ts", "http://host/dir/list.txt").string() << "\"" << std::endl;

    // Linux:   "http://host/dir/"
    // Windows: "http://host/dir/"
    std::cout << "\"http://host/dir/list.txt\".remove_filename() = \"" << fs::path("http://host/dir/list.txt").remove_filename().string() << "\"" << std::endl;

#if defined(__linux__)
    // fs::absolute("/proc/self") = "/proc/self"
    // fs::absolute("/proc/self/net/tcp") = "/proc/self/net/tcp"
    // fs::weakly_canonical("/proc/self") = "/proc/2972"
    // fs::weakly_canonical("/proc/self/exe") = "/home/lelegard/demo-cpp/bin/path"
    // fs::weakly_canonical("/proc/self/net/tcp") = "/proc/2972/net/tcp"
    // fs::weakly_canonical("/proc/self/non/existent") = "/proc/2972/non/existent"
    std::cout << "fs::absolute(\"/proc/self\") = \"" << fs::absolute("/proc/self").string() << "\"" << std::endl;
    std::cout << "fs::absolute(\"/proc/self/net/tcp\") = \"" << fs::absolute("/proc/self/net/tcp").string() << "\"" << std::endl;
    std::cout << "fs::weakly_canonical(\"/proc/self\") = \"" << fs::weakly_canonical("/proc/self").string() << "\"" << std::endl;
    std::cout << "fs::weakly_canonical(\"/proc/self/exe\") = \"" << fs::weakly_canonical("/proc/self/exe").string() << "\"" << std::endl;
    std::cout << "fs::weakly_canonical(\"/proc/self/net/tcp\") = \"" << fs::weakly_canonical("/proc/self/net/tcp").string() << "\"" << std::endl;
    std::cout << "fs::weakly_canonical(\"/proc/self/non/existent\") = \"" << fs::weakly_canonical("/proc/self/non/existent").string() << "\"" << std::endl;
#endif
}
