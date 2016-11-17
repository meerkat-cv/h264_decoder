// Author: Sudeep Pillai (spillai@csail.mit.edu)
// License: BSD
// Last modified: Sep 14, 2014
#define PY_ARRAY_UNIQUE_SYMBOL pbcvt_ARRAY_API
#include "np_opencv_converter.hpp"
#include <numpy/ndarrayobject.h>
#include <string>

namespace fs { namespace python {

// Static PyInit
void* py_init() {
  Py_Initialize();
  import_array();
}

// Singleton init and export converters
static bool export_type_conversions_once = false;
bool init_and_export_converters() {

  if (export_type_conversions_once)
    return false;
  
  export_type_conversions_once = true;

  // Py_Init and array import
  py_init();
  
  // => py::list
  expose_template_type<int>();
  expose_template_type<float>();
  expose_template_type<double>();
  expose_template_type<std::wstring>();

  // std::vector => py::list
  expose_template_type< std::vector<int> >();
  expose_template_type< std::vector<float> >();
  expose_template_type< std::vector<double> >();
  expose_template_type< std::vector< std::vector<int> > >();
  expose_template_type< std::vector< std::vector<double> > >();

  expose_template_type< std::vector<cv::Point> >();
  expose_template_type< std::vector< std::vector<cv::Point> > >();
  expose_template_type< std::vector<cv::Point2f> >();
  expose_template_type< std::vector<cv::KeyPoint> >();
  
  expose_template_type< std::vector<cv::Mat> >();
  expose_template_type< std::vector<cv::Mat_<double> > >();
  expose_template_type< std::vector<cv::Mat1b > >();
  expose_template_type< std::vector<cv::Mat1f > >();

  expose_template_type< std::vector<std::wstring > >();

  // std::map => py::dict
  expose_template_type<std::map<int, std::vector<int> > >();
  expose_template_type<std::map<int, std::vector<float> > >();
  expose_template_type<std::map<std::string, float> >();

  // various converters to cv::Mat
  py::to_python_converter<cv::Point, Point_to_mat>();
  py::to_python_converter<cv::Point2f, Point2f_to_mat>();
  py::to_python_converter<cv::Point3f, Point3f_to_mat>();
  py::to_python_converter<cv::Vec3f, Vec3f_to_mat>();

  
  return true;
}

} // namespace python
} // namespace fs

