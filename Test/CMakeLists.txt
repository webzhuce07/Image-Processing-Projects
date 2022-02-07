CMAKE_MINIMUM_REQUIRED(VERSION 3.12)
cmake_policy(SET CMP0048 NEW)
project(TestCMake VERSION "1.0.0.0")

include_directories(${PROJECT_SOURCE_DIR}/include) # 指定头文件搜索路径

aux_source_directory(${PROJECT_SOURCE_DIR}/src src_dirs) # 指定源码目录

link_directories(${PROJECT_SOURCE_DIR}/lib) # 指定库搜索路径

add_subdirectory(src) # 指定 编译子目录


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib) # 指定可执行文件输出目录
add_executable(fruit ${src_dirs}) # 指定可执行文件名及源码

target_link_libraries(fruit apple pear orange) # 链接外部库

install(TARGETS ${head_files} DESTINATION  ${CMAKE_BINARY_DIR}/include)
install(TARGETS fruit apple pear orange DESTINATION  ${PROJECT_BINARY_DIR}/bin)
