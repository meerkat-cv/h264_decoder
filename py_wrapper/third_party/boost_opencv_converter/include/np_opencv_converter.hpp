// Author: Sudeep Pillai (spillai@csail.mit.edu)
// License: BSD
// Last modified: Sep 14, 2014

#ifndef NP_OPENCV_CONVERTER_HPP_
#define NP_OPENCV_CONVERTER_HPP_

// Boost python includes
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

// Include templated conversion utils
#include "template.h"
#include "container.h"
// #include "utils/conversion.h"

// opencv includes
#include "pyboostcvconverter.hpp"
#include <opencv2/opencv.hpp>

namespace fs { namespace python {

// Vec3f => cv::Mat
struct Vec3f_to_mat {
  static PyObject* convert(const cv::Vec3f& v){
    PyObject* ret = pbcvt::fromMatToNDArray(cv::Mat(v));
    return ret;
  }
};

// cv::Point => cv::Mat
struct Point_to_mat {
  static PyObject* convert(const cv::Point& v){
    PyObject* ret = pbcvt::fromMatToNDArray(cv::Mat(v));
    return ret;
  }
};

// cv::Point2f => cv::Mat
struct Point2f_to_mat {
  static PyObject* convert(const cv::Point2f& v){
    PyObject* ret = pbcvt::fromMatToNDArray(cv::Mat(v));
    return ret;
  }
};

// cv::Point3f => cv::Mat
struct Point3f_to_mat {
  static PyObject* convert(const cv::Point3f& v){
    PyObject* ret = pbcvt::fromMatToNDArray(cv::Mat(v));
    return ret;
  }
};

// cv::Mat_<T> => Numpy PyObject
template <typename T>
struct Mat_to_PyObject {
  static PyObject* convert(const T& mat){
    PyObject* ret = pbcvt::fromMatToNDArray(mat);
    return ret;
  }
};

bool init_and_export_converters();

} // namespace python
} // namespace fs

#endif // NP_OPENCV_CONVERTER_HPP_
