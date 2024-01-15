#include <iostream>

#include "scanner.hpp"
#include "netstat.hpp"
#include "procfs.hpp"

using Source = Options::Source;

Scanner::Scanner(const Options &options)
    : _options(options)
{
}

void Scanner::start() const
{
    Netstat netstat;

    std::cout << "Origin\tPort\tPID\tPath\n";

    netstat.discover([this](const Netstat::Entry &entry)
                     {
                         Process process(entry.pid);

                         if (_options.runtime.debug)
                         {
                             std::cout << entry << std::endl;
                         }

                         if (_options.source.port == entry.local.port && (
                             _options.source.origin == PortOrigin::Local ||
                             _options.source.origin == PortOrigin::Either))
                         {
                             std::cout << "Server" << '\t'
                                       << entry.foreign.port << '\t'
                                       << entry.pid << '\t'
                                       << process.get_filepath() << '\n';
                         }
                         if (_options.source.port == entry.foreign.port && (
                             _options.source.origin == PortOrigin::Remote ||
                             _options.source.origin == PortOrigin::Either))
                         {
                             std::cout << "Client" << '\t'
                                       << entry.local.port << '\t'
                                       << entry.pid << '\t'
                                       << process.get_filepath() << '\n';
                         } });
}
