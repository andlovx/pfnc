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
