cmake_minimum_required(VERSION 3.5)
project (pfnc)
add_executable(pfnc main.cpp options.hpp options.cpp program.hpp program.cpp scanner.hpp)
include_directories(".")

if (CMAKE_SYSTEM_NAME MATCHES "Linux") 
    target_sources(pfnc PUBLIC linux/scanner.cpp)
endif()
