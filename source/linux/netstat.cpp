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

#include <regex>
#include <iostream>

#include "netstat.hpp"
#include "process.hpp"

namespace
{
    class Runner
    {
    public:
        Runner() : handle(popen("netstat -nptu", "r")) {}
        ~Runner() { pclose(handle); }

        operator FILE *() { return handle; }

    private:
        FILE *handle;
    };

    class Buffer : public std::array<char, 128>
    {
    public:
        Buffer();
        bool populate(Netstat::Entry &);

    private:
        std::regex regexp;
    };
}

void Netstat::discover(const std::function<void(const Entry &)> callback) const
{
    Runner process;
    Buffer buffer;
    Entry entry;

    while (std::fgets(buffer.data(), buffer.size(), process))
    {
        if (buffer.populate(entry))
        {
            callback(entry);
        }
    }
}

Buffer::Buffer()
{
    regexp = std::regex("(\\w+).*?([.0-9]+):(\\d+)\\s+([.0-9]+):(\\d+).*?(\\d+)/(\\w+)");
}

bool Buffer::populate(Netstat::Entry &entry)
{
    std::smatch matches;
    std::string subject(data());

    if (!std::regex_search(subject, matches, regexp))
    {
        return false;
    }
    if (matches.size() != 8)
    {
        std::cerr << "Expected 8 matches, got " << matches.size() << std::endl;
        return false;
    }

    entry.proto = matches[1];
    entry.local.addr = matches[2];
    entry.local.port = atoi(matches[3].str().c_str());
    entry.foreign.addr = matches[4];
    entry.foreign.port = atoi(matches[5].str().c_str());
    entry.pid = atoi(matches[6].str().c_str());
    entry.program = matches[7];

    if (entry.pid)
    {
        Process process(entry.pid);
        entry.path = process.get_filepath();
    }

    return true;
}
