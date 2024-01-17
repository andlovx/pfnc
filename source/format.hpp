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
