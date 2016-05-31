#pragma once

/********************************************************************
*			Akhmedov Rolan -- rolan.kharkiv@gmail.com				*
*					18 May 2016 -- version 0.9						*
* 	Camera object stores all the data about a part of the parking	*
*	All coverd spots can be managed only by one camera (v.0.9)		*
********************************************************************/

#include <list>
#include <iterator>
#include "spot.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class Camera {
public:
	Camera();
	Camera(unsigned num, std::list<Spot> spotConfiguration, 
		std::vector<std::vector<cv::Point> > roadMask);

	unsigned getID();
	Spot getSpotEntity(unsigned spotNum);
	std::vector<unsigned> getSpotNumbers();
	SpotStatus getSpotStatus(unsigned spotNum);
	std::string getSpotGPS(unsigned spotNum);
	std::vector<cv::Point> getSpotContour(unsigned spotNum);
	unsigned getDefaultFill(unsigned spotNum);
	void setSpotStatus(unsigned spotNum, SpotStatus sts);
	// RoadStatus getRoadStatus(); TODO: not implemented
	// getVideoStream(); TODO: not implemented
	// getSpotStatusMask(); TODO: not implemented
private:
	std::vector<std::vector<cv::Point> > roadCountur;
	std::list<Spot> spotEntity;
	unsigned number;
};
