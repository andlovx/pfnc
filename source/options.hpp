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

#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <iostream>
#include <string>

enum class PortOrigin
{
    Unspec,
    Local,
    Remote,
    Either
};

enum class OutputFormat
{
    Table,
    JSON,
    XML
};

struct Option
{
    Option(std::string arg);

    std::string key;
    std::string val;
};

struct Options
{
    Options();

    struct Source
    {
        PortOrigin origin;
        uint16_t port;
    } source;

    struct Output
    {
        OutputFormat format = OutputFormat::Table;
        struct List
        {
            bool origin;
            bool proto;
            bool port;
            bool pid;
            bool addr;
            bool path;
        } list;

        void set_listing(const std::string &list);
        void set_format(const std::string &format);
    } output;

    struct Runtime
    {
        bool debug;
        bool verbose;
    } runtime;
};

std::ostream &operator<<(std::ostream &out, const Options &options);

#endif // OPTIONS_HPP
