cmake_minimum_required(VERSION 2.8.3)

set(CMAKE_BUILD_TYPE Release)

find_package( OpenCV 3.1.0 EXACT REQUIRED )

# I am assuming that the builds starts at build/ folder
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib/)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

INCLUDE_DIRECTORIES("../src/src/"
					"../src/inc/")

find_library(H264_LIB NAMES meerkat_h264_decoder
             HINTS ../lib/ )

file(GLOB all_h264_test_src "src/main.cpp")

add_executable(h264_test ${all_h264_test_src})
target_link_libraries(h264_test ${OpenCV_LIBS} ${H264_LIB})