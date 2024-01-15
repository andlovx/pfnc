#ifndef NETSTAT_HPP
#define NETSTAT_HPP

#include <functional>
#include <iostream>

#include <string>

class Netstat
{
public:
    struct Address
    {
        std::string addr;
        uint16_t port;
    };

    struct Entry
    {
        Address local;
        Address foreign;
        int pid;
        std::string program;
        std::string proto;
    };

    void discover(const std::function<void(const Entry &)>) const;
};

inline std::ostream &operator<<(std::ostream &out, const Netstat::Address &address)
{
    return out << '{' << "addr: " << address.addr << ", port: " << address.port << '}';
}

inline std::ostream &operator<<(std::ostream &out, const Netstat::Entry &entry)
{
    return out << '{' << "proto: " << entry.proto << ", pid: " << entry.pid << ", local: " << entry.local << ", foreign: " << entry.foreign << ", program: " << entry.program << '}';
}

#endif // NETSTAT_HPP
