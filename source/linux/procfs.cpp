#include <unistd.h>
#include <limits.h>
#include "procfs.hpp"
#include "error.hpp"

void Process::set_path(int pid)
{
    std::stringstream link;
    link << "/proc/" << pid << "/exe";

    char buff[PATH_MAX];
    int bytes = 0;

    if ((bytes = readlink(link.str().c_str(), buff, sizeof(buff))) == -1)
    {
        error.write("readlink", pid);
        return;
    }

    buff[bytes] = '\0';
    path.assign(buff);
}
