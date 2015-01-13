cmake_minimum_required(VERSION 2.8)

project(Plugin)

find_package(Boost 1.49  REQUIRED COMPONENTS locale date_time thread filesystem system program_options regex)

include_directories(${Boost_INCLUDE_DIRS})

add_definitions(-D_LINUX)

add_subdirectory(src)

add_subdirectory(test)

add_subdirectory(lib)

