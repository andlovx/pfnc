#include <unistd.h>
#include <limits.h>
#include "procfs.hpp"

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
    return path.filename();
}

std::string Process::get_filepath() const
{
    return path;
}

void Process::set_path(int pid)
{
    std::stringstream link;
    link << "/proc/" << pid << "/exe";

    char buff[PATH_MAX];
    int bytes = 0;

    if ((bytes = readlink(link.str().c_str(), buff, sizeof(buff))) == -1)
    {
        perror("readlink");
        return;
    }

    buff[bytes] = '\0';
    path.assign(buff);
}
