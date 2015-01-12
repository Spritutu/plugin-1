cmake_minimum_required(VERSION 2.8)

project(Plugin)

if(UNIX)
set(_LINUX 1)
endif()

add_subdirectory(src)

add_subdirectory(test)

add_subdirectory(lib)
