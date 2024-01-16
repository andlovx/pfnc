#include <iostream>
#include <string>

#if (__linux)
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
{
    parse(argc, argv);
}

void Program::parse(int argc, char **argv)
{
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
        << "  -p,--port=num:  The port number.\n"
        << "  -l,--local:     Port refer to local side.\n"
        << "  -r,--remote:    Port refer to foreign side.\n"
        << "  -d,--debug:     Enabled debug mode.\n"
        << "  -v,--verbose:   Be more verbose.\n"
        << "  -h,--help:      This casual help.\n"
        << "  -V:             Show program version.\n"
        << "\n"
        << "Example:\n"
        << "  " << name() << " -p 43847 -r\n"
        << "  " << name() << " --port=43847 --remote\n"
        << "\n"
        << "Copyright (C) 2023-2024 Xertified AB. "
        << "Released under GNU GPL version 3 license.\n";
}
