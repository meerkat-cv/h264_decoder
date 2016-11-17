#include "boost_opencv_converter.h"
#include "CvRectConverter.h"
#include "np_opencv_converter.hpp"
#define PY_ARRAY_UNIQUE_SYMBOL pbcvt_ARRAY_API
#include "pyboostcvconverter.hpp"



static void *init_ar(){
    Py_Initialize();

    import_array();
}



static bool export_converter_once = false;
bool ExportConverters() {

	if (export_converter_once)
    	return false;

	init_ar();
	fs::python::init_and_export_converters();

	boost::python::to_python_converter<cv::Mat,pbcvt::matToNDArrayBoostConverter>();
	boost::python::to_python_converter<cv::Mat_<double>,pbcvt::matDoubleToNDArrayBoostConverter>();
	pbcvt::matDoubleFromNDArrayBoostConverter();
    pbcvt::matFromNDArrayBoostConverter();

	export_cv_rect_converter();

	export_converter_once = true;
    return true;
	
}
