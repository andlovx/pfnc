// Path for program using network port
// Copyright (C) 2024 Anders Lövgren, Xertified AB
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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
