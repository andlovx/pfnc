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

#include <iostream>

#include "scanner.hpp"
#include "netstat.hpp"
#include "process.hpp"
#include "format.hpp"

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

void Scanner::start(const std::function<void(const Entry &)> callback) const
{
    Netstat netstat;

    netstat.discover([this, &callback](const Netstat::Entry &entry)
                     {
                          if (_options.runtime.debug)
                          {
                             std::cout << entry << std::endl;
                          }

                          if (_options.source.port == entry.local.port && (
                              _options.source.origin == PortOrigin::Local ||
                              _options.source.origin == PortOrigin::Either))
                          {
                              callback(entry);
                          }
                          if (_options.source.port == entry.foreign.port && (
                              _options.source.origin == PortOrigin::Remote ||
                              _options.source.origin == PortOrigin::Either))
                          {
                              callback(entry);
                          } });
}