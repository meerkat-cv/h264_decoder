cmake_minimum_required(VERSION 2.8.3)

set(CMAKE_BUILD_TYPE Release)

if(APPLE)
	set(PYTHON_INCLUDE_DIRS
		"/usr/local/Cellar/python3/3.5.1/Frameworks/Python.framework/Versions/3.5/include/python3.5m")
	set(PYTHON_LIBRARIES
		"/usr/local/Cellar/python3/3.5.1/Frameworks/Python.framework/Versions/3.5/lib/libpython3.5.dylib")
else()
	set(PYTHON_INCLUDE_DIRS "/usr/include/python3.5m/")
	set(PYTHON_LIBRARIES "/usr/lib/x86_64-linux-gnu/libpython3.5m.so")
endif()
	

FIND_PACKAGE(Boost COMPONENTS python)
find_package(OpenCV  REQUIRED )

find_package(PkgConfig REQUIRED)
include(third_party/boost_opencv_converter/boost-python-module.cmake)

# I am assuming that the builds starts at build/ folder
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib/)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -std=c++1y -fPIC -O3")

if(APPLE)
	set(USE_AVX_INSTRUCTIONS true)
	set(NUMPY_INCLUDE_DIR "/usr/local/lib/python3.5/site-packages/numpy/core/include")
elseif(UNIX)
	set(USE_AVX_INSTRUCTIONS true)
	# set(NUMPY_INCLUDE_DIR "/usr/local/lib/python2.7/dist-packages/numpy/core/include/")
endif()

INCLUDE_DIRECTORIES("/opt/local/include/" "../src/src/" "../src/inc/" "../third_party/" "third_party/boost_opencv_converter/include/"
					${PYTHON_INCLUDE_DIRS} ${NUMPY_INCLUDE_DIR})

find_library(H264_DECODER_LIB NAMES meerkat_h264_decoder
             HINTS ../lib/ )

find_library(CONVERTER_LIB NAMES boost_opencv_converter.so
			 HINTS third_party/boost_opencv_converter/lib/)

if(APPLE)
	set(BOOSTPY_LIB "boost_python3")
else()
	set(BOOSTPY_LIB "boost_python-py35")
endif()

boost_python_module_py3(h264_decoder src/h264_decoder_py.cpp)
target_link_libraries(h264_decoder ${CONVERTER_LIB} ${BOOSTPY_LIB} ${PYTHON_LIBRARIES} ${H264_DECODER_LIB} ${OpenCV_LIBS})
