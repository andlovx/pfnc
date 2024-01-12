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
    Table
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
        int port;
    } source;

    struct Output
    {
        OutputFormat format = OutputFormat::Table;
    } output;

    struct Runtime
    {
        bool debug;
        bool verbose;
    } runtime;
};

std::ostream &operator<<(std::ostream &out, const Options &options);

#endif // OPTIONS_HPP
