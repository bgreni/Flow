#include <exception>
#include <stdexcept>
#include <string>

class CodeGenError : std::runtime_error {
public:
    CodeGenError(const std::string & m) : std::runtime_error(m) , msg(m) {}
    const char * what() const noexcept override {
        return msg.c_str();
    }
private:
    std::string msg;
};