message("apple---${PROJECT_SOURCE_DIR}")
set(LIBRARY_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/lib) # 指定动态库输出目录
add_library(apple SHARED ./apple.cpp ${PROJECT_SOURCE_DIR}/include/apple.h) # 生成动态库 libapple.so
