#include <sstream>
#include <exception>
#include "options.hpp"

Options::Options()
{
    source.origin = PortOrigin::Unspec;
    source.port = 0;

    output.format = OutputFormat::Table;
    output.list.addr = false;
    output.list.origin = true;
    output.list.path = true;
    output.list.pid = true;
    output.list.port = true;
    output.list.proto = false;

    runtime.debug = false;
    runtime.verbose = false;
}

Option::Option(std::string arg)
{
    if (std::string::size_type pos = arg.find('='); pos != std::string::npos)
    {
        key = arg.substr(0, pos);
        val = arg.substr(pos + 1);
    }
    else
    {
        key = arg;
    }
}

void Options::Output::set_listing(const std::string &value)
{
    std::istringstream f(value);

    list.addr = list.origin = list.path = list.pid = list.port = list.proto = false;

    for (std::string s; std::getline(f, s, ',');)
    {
        if (s == "origin")
        {
            list.origin = true;
        }
        else if (s == "proto")
        {
            list.proto = true;
        }
        else if (s == "port")
        {
            list.port = true;
        }
        else if (s == "pid")
        {
            list.pid = true;
        }
        else if (s == "addr")
        {
            list.addr = true;
        }
        else if (s == "path")
        {
            list.path = true;
        }
        else if (s == "all")
        {
            list.addr = list.origin = list.path = list.pid = list.port = list.proto = true;
        }
        else
        {
            throw std::invalid_argument("unknown output list value '" + s + "'");
        }
    }
}

void Options::Output::set_format(const std::string &value)
{
    if (value == "table")
    {
        format = OutputFormat::Table;
    }
    else if (value == "json")
    {
        format = OutputFormat::JSON;
    }
    else if (value == "xml")
    {
        format = OutputFormat::XML;
    }
    else
    {
        throw std::invalid_argument("unknown output format value '" + value + "'");
    }
}

std::ostream &operator<<(std::ostream &out, const OutputFormat &format)
{
    switch (format)
    {
    case OutputFormat::Table:
        return out << "Table";
    case OutputFormat::JSON:
        return out << "JSON";
    case OutputFormat::XML:
        return out << "XML";
    default:
        return out << "";
    }
}

std::ostream &operator<<(std::ostream &out, const PortOrigin &origin)
{
    switch (origin)
    {
    case PortOrigin::Unspec:
        return out << "Unspec";
    case PortOrigin::Local:
        return out << "Local";
    case PortOrigin::Remote:
        return out << "Remote";
    case PortOrigin::Either:
        return out << "Either";
    default:
        return out << "";
    }
}

std::ostream &operator<<(std::ostream &out, const Options::Output::List &list)
{
    if (list.addr)
    {
        out << "Address ";
    }
    if (list.origin)
    {
        out << "Origin ";
    }
    if (list.path)
    {
        out << "Path ";
    }
    if (list.pid)
    {
        out << "PID ";
    }
    if (list.port)
    {
        out << "Port ";
    }
    if (list.proto)
    {
        out << "Proto ";
    }

    return out;
}

std::ostream &operator<<(std::ostream &out, const Options &options)
{
    return out
           << "Source:\n"
           << "   Origin: " << options.source.origin << "\n"
           << "     Port: " << options.source.port << "\n"
           << "Output:\n"
           << "   Format: " << options.output.format << "\n"
           << "  Listing: " << options.output.list << "\n"
           << "Runtime:\n"
           << "    Debug: " << options.runtime.debug << "\n"
           << "  Verbose: " << options.runtime.verbose << "\n";
}
