message("orange---${PROJECT_SOURCE_DIR}")
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib) # 指定动态库输出目录
add_library(orange SHARED ./orange.cpp ${PROJECT_SOURCE_DIR}/include/orange.h) # 生成动态库 liborange.so
