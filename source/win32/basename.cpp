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

#include <windows.h>
#include "basename.hpp"

#include <iostream>

extern "C"
{
    char *basename(char *path)
    {
        errno_t result;
        static char copy[MAX_PATH];
        char *pp = strrchr(path, '\\');

        if (!pp)
        {
            return path;
        }

        if ((result = strncpy_s(copy, sizeof(copy), pp + 1, strlen(pp) - 1)) == 0)
        {
            return copy;
        }

        errno = result;
        perror("basename");

        return path;
    }
}
