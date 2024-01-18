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

#ifndef FORMAT_HPP
#define FORMAT_HPP

#include <iostream>
#include "options.hpp"
#include "netstat.hpp"

class Formatter
{
public:
    virtual void print_header(std::ostream &stream) = 0;
    virtual void print_footer(std::ostream &stream) = 0;
    virtual void print_entry(std::ostream &stream, const Netstat::Entry &entry, bool local) = 0;

    static Formatter *create(OutputFormat format, const Options::Output::List &list);

protected:
    Formatter(const Options::Output::List &list);
    const Options::Output::List &list;
};

class JsonFormatter : public Formatter
{
public:
    JsonFormatter(const Options::Output::List &list);

    void print_header(std::ostream &stream) override;
    void print_footer(std::ostream &stream) override;
    void print_entry(std::ostream &stream, const Netstat::Entry &entry, bool local) override;

private:
    void start_object(std::ostream &stream, const char *key);
    void start_object(std::ostream &stream);
    void end_object(std::ostream &stream);

    void print_value(std::ostream &stream, const char *key, int value);
    void print_value(std::ostream &stream, const char *key, const std::string &value);

    bool prev_entry;
    bool prev_key;
};

class XmlFormatter : public Formatter
{
public:
    XmlFormatter(const Options::Output::List &list);

    void print_header(std::ostream &stream) override;
    void print_footer(std::ostream &stream) override;
    void print_entry(std::ostream &stream, const Netstat::Entry &entry, bool local) override;
};

class TableFormatter : public Formatter
{
public:
    TableFormatter(const Options::Output::List &list);

    void print_header(std::ostream &stream) override;
    void print_footer(std::ostream &stream) override;
    void print_entry(std::ostream &stream, const Netstat::Entry &entry, bool local) override;
};

#endif // FORMAT_HPP
