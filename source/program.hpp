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

#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "options.hpp"

class Program
{
public:
    Program();
    Program(char *name);
    Program(std::string name);
    Program(int argc, char **argv);

    void parse(int argc, char **argv);
    void run();

    std::string name() const;
    std::string version() const;
    void version(std::ostream &out) const;

private:
    void usage();

    std::string _name;
    Options _options;
};

#endif
