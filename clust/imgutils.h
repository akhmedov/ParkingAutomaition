#pragma once

/********************************************************************
*			Akhmedov Rolan -- rolan.kharkiv@gmail.com				*
*					18 May 2016 -- version 0.9						*
* 		Some usefull utils for parking video monitoring system		*
*		Open source lib is in use: OpenCV (GNU Licence GPLv2)		*
********************************************************************/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "../parking/camera.h"
#include <iostream>

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
	static unsigned culcFill(const cv::Mat &image, const std::vector<cv::Point> &contur);
	static cv::Mat minimize(cv::Mat src, int scale);
	static void drowSpotStatus(Camera cam1, cv::Mat *screen, int markerSize = 20);
	static cv::Point2f getCounturCenter(std::vector<cv::Point> countur);
	static bool isPointInside(cv::Point2f dot, std::vector<cv::Point> countur);
	unsigned culcFill(const cv::Mat &image, const Spot &spot);
	// color can be any exapt black
	static cv::Mat drawConturs(Conturs contours);
	static cv::Mat drawConturs(Conturs contours, cv::Scalar color);
};
