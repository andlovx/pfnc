#ifndef SCANNER_HPP
#define SCANNER_HPP

#include "options.hpp"

class Scanner
{
public:
    Scanner(const Options &options);
    void start() const;

private:
    const Options &_options;
};

#endif // SCANNER_HPP
