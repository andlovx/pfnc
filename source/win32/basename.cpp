#include <windows.h>
#include "basename.hpp"

#include <iostream>

extern "C"
{
    char *basename(char *path)
    {
        errno_t result;
        static char copy[MAX_PATH];
        char *pp = strrchr(path, '\\');

        if (!pp)
        {
            return path;
        }

        if ((result = strncpy_s(copy, sizeof(copy), pp + 1, strlen(pp) - 1)) == 0)
        {
            return copy;
        }

        errno = result;
        perror("basename");

        return path;
    }
}
