/*
 * CvRectConverter.cpp
 *
 *      Author: Gustavo Fuhr
 *   Copyright: 2016 Meerkat
 *     License: BSD
 */

#include "CvRectConverter.h"

static bool export_cv_rect_conversions_once = false;

void export_cv_rect_converter() {
	if (export_cv_rect_conversions_once)
    	return;
  
	// register the QString-to-python converter
	boost::python::to_python_converter<cv::Rect,CvRect_to_python>();
	boost::python::to_python_converter<std::vector<cv::Rect>, vector_CvRect_to_python>();
	boost::python::to_python_converter< std::vector< std::vector<cv::Rect> >, vector_vector_CvRect_to_python>();
	
	export_cv_rect_conversions_once = true;
}