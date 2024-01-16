cmake_minimum_required(VERSION 3.5)
project (pfnc)
add_executable(pfnc main.cpp options.hpp options.cpp program.hpp program.cpp scanner.hpp scanner.cpp netstat.hpp process.hpp process.cpp error.hpp error.cpp)

cmake_policy(SET CMP0076 NEW)
set_property (TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

include_directories(".")
message("-- Platform: ${CMAKE_SYSTEM_NAME}")

if (CMAKE_SYSTEM_NAME MATCHES "Linux") 
    target_sources(pfnc PUBLIC linux/netstat.cpp linux/procfs.cpp linux/syserr.cpp)
endif()
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    target_sources(pfnc PUBLIC win32/netstat.cpp win32/winproc.cpp win32/basename.hpp win32/basename.cpp win32/syserr.cpp)
    target_link_libraries(pfnc shlwapi)
endif()
