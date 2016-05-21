#include "camera.h"

Camera::Camera(unsigned num, std::list<Spot> spotConfiguration, 
		std::vector<std::vector<cv::Point> > roadMask)
{
	this->number = num;
	this->spotEntity = spotConfiguration;
	this->roadCountur = roadMask;
}

std::vector<unsigned> Camera::getSpotNumbers()
{
	std::vector<unsigned> nums;
	std::list<Spot>::iterator spot;
	for (spot = spotEntity.begin(); spot != spotEntity.end(); ++spot)
		nums.push_back( (spot)->getNumber() );
	return nums;
}

SpotStatus Camera::getSpotStatus(unsigned spotNum)
{
	std::list<Spot>::iterator spot;
	std::advance(spot, spotNum);
	return spot->getStatus();
}
