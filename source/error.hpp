#ifndef ERROR_HPP
#define ERROR_HPP

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <windows.h>
typedef DWORD ErrorCode;
typedef LPTSTR ErrorString;
#else
typedef int ErrorCode;
typedef const char *ErrorString;
#endif

class ErrorLogger
{
public:
    ErrorLogger();

    void silent(bool enable);
    void write(const char *reason);
    void write(const char *reason, int pid);

private:
    bool _silent;
};

struct SystemError
{
    SystemError();
    SystemError(ErrorCode code);
    ~SystemError();

    ErrorCode code;
    ErrorString message;
};

extern ErrorLogger error;

#endif // ERROR_HPP
