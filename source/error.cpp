#include <iostream>
#include "error.hpp"

ErrorLogger error;

std::ostream &operator<<(std::ostream &out, const SystemError &err)
{
    return out << err.message << " (" << err.code << ")";
}

ErrorLogger::ErrorLogger() : _silent(true)
{
}

void ErrorLogger::silent(bool enable)
{
    _silent = enable;
}

void ErrorLogger::write(const char *reason)
{
    if (!_silent)
    {
        std::cerr << reason << ": " << SystemError() << std::endl;
    }
}

void ErrorLogger::write(const char *reason, int pid)
{
    if (!_silent)
    {
        std::cerr << reason << " (" << pid << ")"
                  << ": " << SystemError() << std::endl;
    }
}
