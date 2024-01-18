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

#include "format.hpp"

Formatter::Formatter(const Options::Output::List &list)
    : list(list)
{
}

Formatter *Formatter::create(OutputFormat format, const Options::Output::List &list)
{
    switch (format)
    {
    case OutputFormat::JSON:
        return new JsonFormatter(list);
    case OutputFormat::Table:
        return new TableFormatter(list);
    case OutputFormat::XML:
        return new XmlFormatter(list);
    default:
        return new TableFormatter(list);
    }
}

JsonFormatter::JsonFormatter(const Options::Output::List &list)
    : Formatter(list), prev_entry(false), prev_key(false)
{
}

void JsonFormatter::print_header(std::ostream &stream)
{
    stream << "{\"entries\":[";
}

void JsonFormatter::print_footer(std::ostream &stream)
{
    stream << "]}";
}

void JsonFormatter::print_entry(std::ostream &stream, const Netstat::Entry &entry, bool local)
{
    start_object(stream);

    if (list.origin)
    {
        print_value(stream, "origin", local ? "client" : "server");
    }
    if (list.proto)
    {
        print_value(stream, "proto", entry.proto);
    }
    if (list.port)
    {
        print_value(stream, "port", local ? entry.local.port : entry.foreign.port);
    }

    if (list.pid)
    {
        print_value(stream, "pid", entry.pid);
    }
    if (list.addr)
    {
        start_object(stream, "addr");
        prev_key = false;
        start_object(stream, "local");
        print_value(stream, "ip", entry.local.addr);
        print_value(stream, "port", entry.local.port);
        end_object(stream);
        start_object(stream, "remote");
        prev_key = false;
        print_value(stream, "ip", entry.foreign.addr);
        print_value(stream, "port", entry.foreign.port);
        end_object(stream);
        end_object(stream);
    }
    if (list.path)
    {
        print_value(stream, "path", entry.path);
    }

    end_object(stream);
}

void JsonFormatter::start_object(std::ostream &stream, const char *key)
{
    if (prev_key)
    {
        stream << ",";
    }

    stream << "\"" << key << "\":{";
}

void JsonFormatter::start_object(std::ostream &stream)
{
    prev_key = false;

    if (prev_entry)
    {
        stream << ",";
    }

    stream << "{";
}

void JsonFormatter::end_object(std::ostream &stream)
{
    stream << "}";
    prev_entry = true;
}

void JsonFormatter::print_value(std::ostream &stream, const char *key, int value)
{
    if (prev_key)
    {
        stream << ",";
    }

    stream << "\"" << key << "\":" << value;

    if (!prev_key)
    {
        prev_key = true;
    }
}

void JsonFormatter::print_value(std::ostream &stream, const char *key, const std::string &value)
{
    if (prev_key)
    {
        stream << ",";
    }

    stream << "\"" << key << "\":\"" << value << "\"";

    if (!prev_key)
    {
        prev_key = true;
    }
}

TableFormatter::TableFormatter(const Options::Output::List &list)
    : Formatter(list)
{
}

void TableFormatter::print_header(std::ostream &stream)
{
    if (list.origin)
    {
        stream << "Origin\t";
    }
    if (list.proto)
    {
        stream << "Proto\t";
    }
    if (list.port)
    {
        stream << "Port\t";
    }

    if (list.pid)
    {
        stream << "PID\t";
    }
    if (list.addr)
    {
        stream << "Local\tRemote\t";
    }
    if (list.path)
    {
        stream << "Path\t";
    }

    stream << "\n";
}

void TableFormatter::print_footer(std::ostream &stream)
{
}

void TableFormatter::print_entry(std::ostream &stream, const Netstat::Entry &entry, bool local)
{
    if (list.origin)
    {
        stream << (local ? "Client" : "Server")
               << "\t";
    }
    if (list.proto)
    {
        stream << entry.proto << "\t";
    }
    if (list.port)
    {
        stream << (local ? entry.local.port : entry.foreign.port) << "\t";
    }

    if (list.pid)
    {
        stream << entry.pid << "\t";
    }
    if (list.addr)
    {
        stream << entry.local.addr << ":" << entry.local.port << "\t"
               << entry.foreign.addr << ":" << entry.foreign.port << "\t";
    }
    if (list.path)
    {
        stream << entry.path << "\t";
    }

    stream << "\n";
}

XmlFormatter::XmlFormatter(const Options::Output::List &list)
    : Formatter(list)
{
}

void XmlFormatter::print_header(std::ostream &stream)
{
    stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    stream << "<entries generator=\"pfnc\">\n";
}

void XmlFormatter::print_footer(std::ostream &stream)
{
    stream << "</entries>\n";
}

void XmlFormatter::print_entry(std::ostream &stream, const Netstat::Entry &entry, bool local)
{
    stream << "  <entry>\n";

    if (list.origin)
    {
        stream << "    <origin>" << (local ? "client" : "server") << "</origin>\n";
    }
    if (list.proto)
    {
        stream << "    <proto>" << entry.proto << "</proto>\n";
    }
    if (list.port)
    {
        stream << "    <port>" << (local ? entry.local.port : entry.foreign.port) << "</port>\n";
    }

    if (list.pid)
    {
        stream << "    <pid>" << entry.pid << "</pid>\n";
    }
    if (list.addr)
    {
        stream << "    <addr>\n";
        stream << "      <local>\n";
        stream << "        <ip>" << entry.local.addr << "</ip>\n";
        stream << "        <port>" << entry.local.port << "</ip>\n";
        stream << "      </local>\n";
        stream << "      <remote>\n";
        stream << "        <ip>" << entry.local.addr << "</ip>\n";
        stream << "        <port>" << entry.local.port << "</ip>\n";
        stream << "      </remote>\n";
        stream << "    </addr>\n";
    }
    if (list.path)
    {
        stream << "    <path>" << entry.path << "</path>\n";
    }

    stream << "  </entry>\n";
}
