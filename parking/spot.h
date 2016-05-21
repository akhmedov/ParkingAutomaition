#pragma once

/********************************************************************
*			Akhmedov Rolan -- rolan.kharkiv@gmail.com				*
*					19 May 2016 -- version 0.9						*
* 						Parking Spot Entity							*
********************************************************************/

#include <vector>
#include <string>
#include "opencv2/highgui/highgui.hpp"

enum SpotStatus {vacant, busy, unknown};

class Spot {
public:
	Spot(unsigned num, std::vector<cv::Point> ctr, std::string gps);
	void setDefaultFill(unsigned pixelCount);
	void setStatus(SpotStatus sts);
	void track();
	void untrack();

	unsigned getDefaultFill();
	std::vector<cv::Point> getCountur();
	unsigned getNumber();
	bool isTrackable();
	SpotStatus getStatus();

private:
	unsigned defaultFill;
	std::vector<cv::Point> countur;
	unsigned number;
	bool trackable;
	SpotStatus status;
	std::string gpsLocation;
};
