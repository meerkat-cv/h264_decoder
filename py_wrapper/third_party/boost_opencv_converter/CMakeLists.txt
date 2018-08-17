cmake_minimum_required(VERSION 2.8.3)

set(CMAKE_BUILD_TYPE Release)

if(APPLE)
	set(PYTHON_INCLUDE_DIRS
		"/usr/local/Cellar/python3/3.6.0_1/Frameworks/Python.framework/Versions/3.6/include/python3.6m/")
	set(PYTHON_LIBRARIES
		"/usr/local/Cellar/python3/3.6.0_1/Frameworks/Python.framework/Versions/3.6/lib/libpython3.6.dylib")
else()
	set(PYTHON_INCLUDE_DIRS "/usr/include/python3.5m/")
	set(PYTHON_LIBRARIES "/usr/lib/x86_64-linux-gnu/libpython3.5m.so")
endif()

FIND_PACKAGE(Boost COMPONENTS python)
find_package( OpenCV  REQUIRED )

find_package(PkgConfig REQUIRED)
include(boost-python-module.cmake)

# I am assuming that the builds starts at build/ folder
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib/)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -std=c++1y -fPIC -O3")

if(APPLE)
	set(NUMPY_INCLUDE_DIR "/usr/local/lib/python3.6/site-packages/numpy/core/include")
	link_directories("/usr/local/Cellar/boost-python/1.60.0/lib/" "/usr/local/Cellar/boost/1.60.0_1/lib/")
elseif(UNIX)
	set(NUMPY_INCLUDE_DIR "/usr/local/lib/python3.5/dist-packages/numpy/core/include/")
endif()


INCLUDE_DIRECTORIES("include/" ${PYTHON_INCLUDE_DIRS} ${NUMPY_INCLUDE_DIR})

file (GLOB all_converter_source
			src/*.cpp
			src/*.h)

if(APPLE)
	set(BOOSTPY_LIB "boost_python3")
else()
	set(BOOSTPY_LIB "boost_python-py35")
endif()

boost_python_module_py3(boost_opencv_converter ${all_converter_source})
target_link_libraries(boost_opencv_converter boost_system ${BOOSTPY_LIB} ${OpenCV_LIBS})
