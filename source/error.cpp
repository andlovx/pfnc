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
