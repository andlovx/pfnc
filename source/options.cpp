#include "options.hpp"

Options::Options()
{
    source.origin = PortOrigin::Unspec;
    source.port = 0;

    output.format = OutputFormat::Table;

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

std::ostream &operator<<(std::ostream &out, const OutputFormat &format)
{
    switch (format)
    {
    case OutputFormat::Table:
        return out << "Table";
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

std::ostream &operator<<(std::ostream &out, const Options &options)
{
    return out
           << "Source:\n"
           << "   Origin: " << options.source.origin << "\n"
           << "     Port: " << options.source.port << "\n"
           << "Output:\n"
           << "   Format: " << options.output.format << "\n"
           << "Runtime:\n"
           << "    Debug: " << options.runtime.debug << "\n"
           << "  Verbose: " << options.runtime.verbose << "\n";
}
