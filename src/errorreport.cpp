#include <iostream>
#include <system_error>
#include <filesystem>
#include <cstdlib>
#include <cerrno>

namespace fs = std::filesystem;

// A subclass of std::error_code which reports error on destructor.
// Create each instance in the system library call to have the destructor called on return.
class error_report: public std::error_code
{
private:
    std::ostream& _out;
    const std::string& _message;
    const std::string& _object;
public:
    error_report(std::ostream& out = std::cout,
                 const std::string& message = std::string(),
                 const std::string& object = std::string()) :
        _out(out), _message(message), _object(object)
    {
    }
    error_report(error_report&) = delete;
    error_report(error_report&&) = delete;
    error_report operator=(error_report&) = delete;
    error_report operator=(error_report&&) = delete;
    
    ~error_report();

    // Turn a constructor expression into an lvalue, used in filesystem calls.
    error_report& operator &() { return *this; }
};

error_report::~error_report()
{
    if (value() != 0) {
        std::string s(_message);
        if (!s.empty() && !_object.empty()) {
            s.append(" ");
        }
        if (!_object.empty()) {
            s.append(_object);
        }
        if (!s.empty()) {
            s.append(": ");
        }
        s.append(message());
        _out << s << std::endl;
    }
}

int main(int argc, char* argv[])
{
    fs::path dir(fs::temp_directory_path());
    fs::path nodir(dir);
    nodir /= "nonexistent";
    
    std::cout << "is_directory(\"" << dir << "\")" << std::endl;
    bool status = fs::is_directory(dir, &error_report(std::cout, "isdir", dir));
    std::cout << "result: " << status << std::endl << std::endl;
    
    std::cout << "is_directory(\"" << nodir << "\")" << std::endl;
    status = fs::is_directory(nodir, &error_report(std::cout, "isdir", nodir));
    std::cout << "result: " << status << std::endl;
}
