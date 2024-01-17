#include "format.hpp"
#include "process.hpp"

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
        Process process(entry.pid);
        print_value(stream, "path", process.get_filepath());
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
}

void TableFormatter::print_footer(std::ostream &stream)
{
}

void TableFormatter::print_entry(std::ostream &stream, const Netstat::Entry &entry, bool local)
{
}

XmlFormatter::XmlFormatter(const Options::Output::List &list)
    : Formatter(list)
{
}

void XmlFormatter::print_header(std::ostream &stream)
{
}

void XmlFormatter::print_footer(std::ostream &stream)
{
}

void XmlFormatter::print_entry(std::ostream &stream, const Netstat::Entry &entry, bool local)
{
}
