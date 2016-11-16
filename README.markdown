# Simple H264 Decoder

This is a simple C++ h264 stream decoder. It is quite fast and more importantly, does not require any other libs to compile/use. The idea is to be easy to use and import to your project, without all the problems of seting up a larger lib like ffstream, gstreamer or libvlc. You just need to compile, add the Decoder.h header (which contains all the interface that you need) and link the libmeerkat_h264_decoder lib.

The idea came from a project that imported the h264 decoder to JavaScript called [Broadway][Broadway_site], which is quite amazing. They used the decoder code from the [Android project][Android_src], but they did an abstraction layer on top (you'll see on all the functions calls that start with broadway). We've used this abstraction layer to make things even easier, but with some small changes for our purpose.

We also will add a python wrapper for this lib, which is quite easy to use and compile. However you'll need OpenCV 3.1 and Boost.

Hope you like it :)

[Meerkat team][Meerkat_site]


### Warnings

This was done on a real tight deadline, so there are some problems/limitations that you must be aware. Some of them:

* The video must be in color
* The code interface should be improved

### Compilation

To compile the main lib:

```sh
$ mkdir build
$ cd build
$ cmake ..
$ make
```

To compile the test program (it needs OpenCV to generate a RGB image and plot on the screen):

```sh
$ cd test
$ mkdir build
$ cd build
$ cmake ..
$ make
```

Usage example:

```sh
$ ./h264_test ../../data/video_full.avi
```

[Broadway_site]: <https://github.com/mbebenita/Broadway>
[Android_src]: <https://android.googlesource.com/platform/frameworks/av/+/master/media/libstagefright/codecs/on2/h264dec/>
[Meerkat_site]: <http://www.meerkat.com.br/?setLng=en-US>