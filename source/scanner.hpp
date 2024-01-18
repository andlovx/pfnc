#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <functional>
#include "options.hpp"
#include "netstat.hpp"

class Scanner
{
    using Entry = Netstat::Entry;

public:
    Scanner(const Options &options);
    void start() const;
    void start(const std::function<void(const Entry &)>) const;

private:
    const Options &_options;
};

#endif // SCANNER_HPP
