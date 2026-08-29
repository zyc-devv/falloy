#include <iostream>
#include <runtime_error>
#include <stdexcept>

class FalloyRuntimeError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};