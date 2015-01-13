set(executable_output_path ${project_source_dir}/bin)

link_directories(${project_source_dir}/lib)

aux_source_directory(. DIR_SRCS)

add_definitions(-std=c++11)

add_executable(plugin_bin ${DIR_SRCS})

link_directories(${Boost_LIB_DIRS})

target_link_libraries(plugin_bin Plugin dl ${Boost_LIBRARIES})
