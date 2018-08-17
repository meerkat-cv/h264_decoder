mkdir build
rm -rf build/*
cd build/
cmake -DUSE_AVX_INSTRUCTIONS=TRUE ..
make -j4
cd ../

cd py_wrapper/third_party/boost_opencv_converter/
mkdir build
rm -rf build/*
cd build/
cmake ..
make -j4
cd ../../../..

cd py_wrapper/
mkdir build
rm -rf build/*
cd build/
cmake -DUSE_AVX_INSTRUCTIONS=TRUE ..
make -j4
