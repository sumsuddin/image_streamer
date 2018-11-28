cmake_minimum_required(VERSION 3.10)
project(image_streamer)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV COMPONENTS core imgproc highgui videoio)
set(IMAGE_STREAMER_INCLUDE_DIR "include")
include_directories(${OpenCV_INCLUDE_DIRS} ${IMAGE_STREAMER_INCLUDE_DIR})

add_executable(image_streamer_exe
        main.cpp
        src/input.cpp
        src/output.cpp
        src/httpd.cpp
        src/image_streamer.cpp)

add_library(image_streamer SHARED
        src/input.cpp
        src/output.cpp
        src/httpd.cpp
        src/image_streamer.cpp)

target_link_libraries(image_streamer_exe ${OpenCV_LIBS} pthread dl)