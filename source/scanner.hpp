#ifndef SCAN_HPP
#define SCAN_HPP

#include "options.hpp"

class Scanner
{
public:
    Scanner(const Options &options);
    void start() const;

private:
    const Options &_options;
};

#endif // SCAN_HPP
