message("pear---${PROJECT_SOURCE_DIR}")
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib) # 指定动态库输出目录
add_library(pear SHARED ./pear.cpp ${PROJECT_SOURCE_DIR}/include/pear.h) # 生成动态库 libpear.so
