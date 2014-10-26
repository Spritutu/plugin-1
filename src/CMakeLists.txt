aux_source_directory(. DIR_LIB_SRCS)
add_definitions(-std=c++11)
add_library(Plugin ${DIR_LIB_SRCS})
