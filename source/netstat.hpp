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

#ifndef NETSTAT_HPP
#define NETSTAT_HPP

#include <iostream>
#include <string>
#include <functional>

class Netstat
{
public:
    struct Address
    {
        std::string addr;
        uint16_t port;
    };

    struct Entry
    {
        Address local;
        Address foreign;
        int pid;
        std::string program;
        std::string path;
        std::string proto;
    };

    void discover(const std::function<void(const Entry &)>) const;
};

inline std::ostream &operator<<(std::ostream &out, const Netstat::Address &address)
{
    return out << '{'
               << "addr: " << address.addr << ", "
               << "port: " << address.port
               << '}';
}

inline std::ostream &operator<<(std::ostream &out, const Netstat::Entry &entry)
{
    return out << '{'
               << "proto: " << entry.proto << ", "
               << "pid: " << entry.pid << ", "
               << "local: " << entry.local << ", "
               << "foreign: " << entry.foreign << ", "
               << "program: " << entry.program << ", "
               << "path: " << entry.path
               << '}';
}

#endif // NETSTAT_HPP
