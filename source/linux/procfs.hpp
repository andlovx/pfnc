#ifndef PROC_HPP
#define PROC_HPP

#include <string>
#include <filesystem>

class Process
{
public:
    Process(int pid);

    const std::filesystem::path &get_path() const;
    std::string get_filename() const;
    std::string get_filepath() const;

    operator std::string() { return path; }

private:
    void set_path(int pid);
    std::filesystem::path path;
};

#endif // PROC_HPP
