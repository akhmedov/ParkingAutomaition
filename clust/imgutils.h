#pragma once

/********************************************************************
*			Akhmedov Rolan -- rolan.kharkiv@gmail.com				*
*					18 May 2016 -- version 0.9						*
* 	CV approach is based on k-means clustering and ... conturing	*
*		Open source lib is in use: OpenCV (GNU Licence GPLv2)		*
********************************************************************/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

struct Conturs {
	Conturs(cv::Mat e, std::vector<cv::Vec4i> h, 
		std::vector<std::vector<cv::Point> > l) 
	{
		this->edges = e;
		this->hierarchy = h;
		this->location = l;
	}
	cv::Mat edges;
	std::vector<cv::Vec4i> hierarchy;
	std::vector<std::vector<cv::Point> > location; 
};

class ImgUtils {
public:
	static cv::Mat minimize(cv::Mat src, int scale);
	// color can be any exapt black
	static cv::Mat drawConturs(Conturs contours);
};
