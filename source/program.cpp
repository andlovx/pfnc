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
#include <string>

#if (__linux) || (__APPLE__)
#include <libgen.h>
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include "win32/basename.hpp"
#endif

#include "program.hpp"
#include "scanner.hpp"
#include "error.hpp"

Program::Program()
{
}

Program::Program(char *name)
    : _name(basename(name))
{
}

Program::Program(std::string name)
    : _name(name)
{
}

Program::Program(int argc, char **argv)
    : _name(basename(argv[0]))
{
    parse(argc, argv);
}

void Program::parse(int argc, char **argv)
{
    if (_name.length() == 0)
    {
        _name = basename(argv[0]);
    }
    if (argc == 1)
    {
        usage();
        exit(1);
    }

    for (int i = 1; i < argc; ++i)
    {
        Option option(argv[i]);

        auto next = [&i, argc, argv]() -> std::string
        {
            if (i + 1 < argc && argv[i + 1][0] != '-')
            {
                return argv[++i];
            }

            return "";
        };

        //
        // Generic options:
        //
        if (option.key == "-h" ||
            option.key == "--help")
        {
            usage();
            exit(0);
        }
        else if (option.key == "-V" ||
                 option.key == "--version")
        {
            version(std::cout);
            exit(1);
        }
        else if (option.key == "-d" ||
                 option.key == "--debug")
        {
            _options.runtime.debug = true;
        }
        else if (option.key == "-v" ||
                 option.key == "--verbose")
        {
            _options.runtime.verbose = true;
        }
        //
        // Output options:
        //
        else if (option.key == "-f")
        {
            _options.output.set_format(next().c_str());
        }
        else if (option.key == "--format")
        {
            _options.output.set_format(option.val);
        }
        else if (option.key == "-o")
        {
            _options.output.set_listing(next().c_str());
        }
        else if (option.key == "--output")
        {
            _options.output.set_listing(option.val);
        }
        else if (option.key == "-T" ||
                 option.key == "--table")
        {
            _options.output.set_format("table");
        }
        else if (option.key == "-J" ||
                 option.key == "--json")
        {
            _options.output.set_format("json");
        }
        else if (option.key == "-X" ||
                 option.key == "--xml")
        {
            _options.output.set_format("xml");
        }
        else if (option.key == "-A" ||
                 option.key == "--all")
        {
            _options.output.set_listing("all");
        }
        //
        // Source options:
        //
        else if (option.key == "-p")
        {
            _options.source.port = atoi(next().c_str());
        }
        else if (option.key == "--port")
        {
            _options.source.port = atoi(option.val.c_str());
        }
        else if (option.key == "-l" ||
                 option.key == "--local")
        {
            _options.source.origin = _options.source.origin == PortOrigin::Unspec ? PortOrigin::Local : PortOrigin::Either;
        }
        else if (option.key == "-r" ||
                 option.key == "--remote")
        {
            _options.source.origin = _options.source.origin == PortOrigin::Unspec ? PortOrigin::Remote : PortOrigin::Either;
        }
        else
        {
            throw std::invalid_argument("option '" + option.key + "' was unexpected");
        }
    }

    if (_options.source.origin == PortOrigin::Unspec)
    {
        _options.source.origin = PortOrigin::Either;
    }
    if (_options.runtime.debug)
    {
        std::cout << _options << "\n";
    }
    if (_options.runtime.verbose)
    {
        error.silent(false);
    }
}

void Program::run()
{
    Scanner scanner(_options);
    scanner.start();
}

std::string Program::name() const
{
    return _name;
}

std::string Program::version() const
{
    return "1.0.0";
}

void Program::version(std::ostream &out) const
{
    out << name() << " v" << version() << std::endl;
}

void Program::usage()
{
    std::cout
        << "List program path using network connection\n"
        << "\n"
        << "Usage: " << name() << " -p num [...options]\n"
        << "\n"
        << "Options:\n"
        << "  -p,--port=num:    The port number.\n"
        << "  -l,--local:       Port refer to local side.\n"
        << "  -r,--remote:      Port refer to foreign side.\n"
        << "  -f,--format=str:  Output format (table,json,xml).\n"
        << "  -o,--output=list: Output listing (origin,proto,port,pid,addr,path,all).\n"
        << "  -d,--debug:       Enabled debug mode.\n"
        << "  -v,--verbose:     Be more verbose.\n"
        << "  -h,--help:        This casual help.\n"
        << "  -V:               Show program version.\n"
        << "Alias:\n"
        << "  -T,--table:       Alias for --format=table\n"
        << "  -J,--json:        Alias for --format=json\n"
        << "  -X,--xml:         Alias for --format=xml\n"
        << "  -A,--all:         Alias for --output=all\n"
        << "\n"
        << "Example:\n"
        << "  " << name() << " -p 43847 -r\n"
        << "  " << name() << " --port=43847 --remote\n"
        << "  " << name() << " --port=43847 -o path,addr,proto -f json\n"
        << "  " << name() << " --port=43847 -o all -f json\n"
        << "\n"
        << "Copyright (C) 2024 Xertified AB. "
        << "Released under GNU GPL version 3 license.\n";
}
