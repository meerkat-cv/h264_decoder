/*
 * CvRectConverter.h
 *
 *      Author: Gustavo Fuhr
 *   Copyright: 2016 Meerkat
 *     License: BSD
 */


#include <iostream>
#include <opencv2/opencv.hpp>
#include <boost/python.hpp>
#include <boost/python/tuple.hpp>
#include <boost/python/slice.hpp>

namespace py = boost::python;

struct CvRect_to_python
{
    static PyObject* convert(cv::Rect const& r)
    {
        // the tuple corresponding to a rect should be ((tl.x, tl.y), (br.x, br.y))
    	py::tuple tl = py::make_tuple(r.tl().x, r.tl().y);
        py::tuple br = py::make_tuple(r.br().x, r.br().y);
        py::tuple t = py::make_tuple(tl, br);
    	
    	return py::incref(t.ptr());
    }
};

struct vector_CvRect_to_python
{
    static PyObject* convert(std::vector<cv::Rect> const& l)
    {
    	py::list py_list;
    	for (int i = 0; i < l.size(); ++i) {
            py::tuple tl = py::make_tuple(l[i].tl().x, l[i].tl().y);
            py::tuple br = py::make_tuple(l[i].br().x, l[i].br().y);
            py::tuple t = py::make_tuple(tl, br);
    		py_list.append(t);
    	}
    	
    	return py::incref(py_list.ptr());
    }
};

struct vector_vector_CvRect_to_python
{
    static PyObject* convert(std::vector< std::vector<cv::Rect> > const& l)
    {
        py::list py_list;
        for (int i = 0; i < l.size(); ++i) {
            py::list inner_list;

            for(int j=0; j<l.at(i).size(); j++) {
                py::tuple tl = py::make_tuple(l[i][j].tl().x, l[i][j].tl().y);
                py::tuple br = py::make_tuple(l[i][j].br().x, l[i][j].br().y);
                py::tuple t = py::make_tuple(tl, br);
                inner_list.append(t);
            }

            py_list.append(inner_list);
        }
        
        return py::incref(py_list.ptr());
    }
};


void export_cv_rect_converter();