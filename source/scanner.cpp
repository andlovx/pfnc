#include <iostream>

#include "scanner.hpp"
#include "netstat.hpp"
#include "process.hpp"
#include "format.hpp"

using Source = Options::Source;

Scanner::Scanner(const Options &options)
    : _options(options)
{
}

void Scanner::start() const
{
    std::unique_ptr<Formatter> formatter(Formatter::create(_options.output.format, _options.output.list));
    Netstat netstat;

    formatter->print_header(std::cout);

    netstat.discover([this, &formatter](const Netstat::Entry &entry)
                     {
                          if (_options.runtime.debug)
                          {
                             std::cout << entry << std::endl;
                          }

                          if (_options.source.port == entry.local.port && (
                              _options.source.origin == PortOrigin::Local ||
                              _options.source.origin == PortOrigin::Either))
                          {
                             formatter->print_entry(std::cout, entry, false);
                          }
                          if (_options.source.port == entry.foreign.port && (
                              _options.source.origin == PortOrigin::Remote ||
                              _options.source.origin == PortOrigin::Either))
                          {
                             formatter->print_entry(std::cout, entry, true);
                          } });

    formatter->print_footer(std::cout);
}
