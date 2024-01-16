#include "error.hpp"

SystemError::SystemError() : SystemError(errno)
{
}

SystemError::SystemError(int code) : code(code)
{
    message = strerror(code);
}

SystemError::~SystemError()
{
}
