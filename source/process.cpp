#include "process.hpp"

Process::Process(int pid)
{
    set_path(pid);
}

const std::filesystem::path &Process::get_path() const
{
    return path;
}

std::string Process::get_filename() const
{
    return path.filename().string();
}

std::string Process::get_filepath() const
{
    return path.string();
}
