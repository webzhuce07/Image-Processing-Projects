# project name
PROJECT(BicubicInterpolation)
# requirement of cmake version
cmake_minimum_required(VERSION 3.5)

# set the directory of executable files
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# find required opencv
#set(OpenCV_DIR G:\\OpenCV\\bin)
find_package(OpenCV 3.2.0 REQUIRED)

# directory of opencv headers
include_directories(${OpenCV_INCLUDE_DIRS})
# name of executable file and path of source file
add_executable(BicubicInterpolation main.cpp)
# directory of opencv library
link_directories(${OpenCV_LIBRARY_DIRS})
# opencv libraries
target_link_libraries(BicubicInterpolation ${OpenCV_LIBS})