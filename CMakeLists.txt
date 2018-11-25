cmake_minimum_required(VERSION 3.10)
project(image_streamer)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV COMPONENTS core imgproc highgui videoio)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(image_streamer main.cpp input.cpp output.cpp httpd.cpp image_streamer.cpp)

target_link_libraries(image_streamer ${OpenCV_LIBS} pthread dl)