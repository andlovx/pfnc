#include <cstring>
#include <cerrno>
#include "error.hpp"

SystemError::SystemError() : SystemError(errno)
{
}

SystemError::SystemError(ErrorCode code) : code(code)
{
    message = strerror(code);
}

SystemError::~SystemError()
{
}
