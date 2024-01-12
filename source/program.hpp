#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "options.hpp"

class Program
{
public:
    Program();
    Program(char *name);
    Program(std::string name);
    Program(int argc, char **argv);

    void parse(int argc, char **argv);
    void run();

    std::string name() const;
    std::string version() const;
    void version(std::ostream &out) const;

private:
    void usage();

    std::string _name;
    Options _options;
};

#endif
