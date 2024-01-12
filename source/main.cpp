#include <iostream>
#include "program.hpp"

int main(int argc, char **argv)
{
    Program program(argv[0]);

    try
    {
        program.parse(argc, argv);
        program.run();
    }
    catch (const std::exception &exception)
    {
        std::cerr << program.name() << ": error: " << exception.what() << std::endl;
    }

    return 0;
}
