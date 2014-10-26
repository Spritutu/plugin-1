aux_source_directory(. DIR_SRCS)

add_definitions(-std=c++11)

add_executable(plugin_bin ${DIR_SRCS})

target_link_libraries(plugin_bin Plugin)
