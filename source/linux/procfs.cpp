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

#include <unistd.h>
#include <limits.h>
#include "process.hpp"
#include "error.hpp"

void Process::set_path(int pid)
{
    std::stringstream link;
    link << "/proc/" << pid << "/exe";

    char buff[PATH_MAX];
    int bytes = 0;

    if ((bytes = readlink(link.str().c_str(), buff, sizeof(buff))) == -1)
    {
        error.write("readlink", pid);
        return;
    }

    buff[bytes] = '\0';
    path.assign(buff);
}
