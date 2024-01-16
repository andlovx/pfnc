#include "error.hpp"

SystemError::SystemError() : SystemError(GetLastError())
{
}

SystemError::SystemError(DWORD code) : code(code), message('\0')
{
    DWORD dwFlags = FORMAT_MESSAGE_FROM_SYSTEM |
                    FORMAT_MESSAGE_ALLOCATE_BUFFER |
                    FORMAT_MESSAGE_IGNORE_INSERTS;
    DWORD dwLength = FormatMessage(
        dwFlags,
        0, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&message, 0, 0);

    if (dwLength)
    {
        message[dwLength - 3] = '\0';
    }
}

SystemError::~SystemError()
{
    LocalFree(message);
}
