#include "Decoder.h"
#include <boost/python.hpp>
#include <boost/python/str.hpp>
#include <boost_opencv_converter.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

namespace py = boost::python;


cv::Mat Stream_getFrame(Stream& self) {
    u32 height, width;
    u8* img_data;

    img_data = self.GetFrame(&width, &height);

    cv::Mat img_buffer(height+height/2, width, CV_8UC1, (uchar *)img_data);
    cv::Mat original;
    cv::cvtColor(img_buffer, original, cv::COLOR_YUV2RGB_YV12);

    return original;
}

int Stream_broadwayDecode(Stream& self) {
    StreamStatus status = self.BroadwayDecode();

    return (int)status;
}



void Stream_setStream(Stream& self, const cv::Mat& strmBuffer) {
    u8* buffer = strmBuffer.data;
    u32 buff_length = (u32)strmBuffer.rows;

    self.SetStream(buffer, buff_length);
}

void Stream_updateStream(Stream& self, const cv::Mat& strmBuffer) {
    u8* buffer = strmBuffer.data;
    u32 buff_length = (u32)strmBuffer.rows;

    self.UpdateStream(buffer, buff_length);
}


BOOST_PYTHON_MODULE(h264_decoder) 
{
    ExportConverters();
    
    py::class_<Stream, boost::noncopyable>("Stream")
        .def("GetFrame", &Stream_getFrame)
        .def("BroadwayDecode", &Stream_broadwayDecode)
        .def("SetStream", &Stream_setStream)
        .def("UpdateStream", &Stream_updateStream)
    ;

}