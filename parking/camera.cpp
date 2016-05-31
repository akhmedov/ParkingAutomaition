#include "camera.h"

Camera::Camera()
{
	
}

Camera::Camera(unsigned num, std::list<Spot> spotConfiguration, 
		std::vector<std::vector<cv::Point> > roadMask)
{
	this->number = num;
	this->spotEntity = spotConfiguration;
	this->roadCountur = roadMask;
}

Spot Camera::getSpotEntity(unsigned spotNum)
{
	std::list<Spot>::iterator spot = spotEntity.begin();
	std::advance(spot, spotNum);
	return *spot;
}

std::vector<unsigned> Camera::getSpotNumbers()
{
	std::vector<unsigned> nums;
	std::list<Spot>::iterator spot;
	for (spot = spotEntity.begin(); spot != spotEntity.end(); ++spot)
		nums.push_back( spot->getNumber() );
	return nums;
}

SpotStatus Camera::getSpotStatus(unsigned spotNum)
{
	std::list<Spot>::iterator spot = spotEntity.begin();
	std::advance(spot, spotNum);
	return spot->getStatus();
}

std::string Camera::getSpotGPS(unsigned spotNum)
{
	std::list<Spot>::iterator spot = spotEntity.begin();
	std::advance(spot, spotNum);
	return spot->getGPS();
}

unsigned Camera::getDefaultFill(unsigned spotNum)
{
	std::list<Spot>::iterator spot = spotEntity.begin();
	std::advance(spot, spotNum);
	return spot->getDefaultFill();
}

void Camera::setSpotStatus(unsigned spotNum, SpotStatus sts)
{
	std::list<Spot>::iterator spot = spotEntity.begin();
	std::advance(spot, spotNum);
	spot->setStatus(sts);
}

std::vector<cv::Point> Camera::getSpotContour(unsigned spotNum)
{
	std::list<Spot>::iterator spot = spotEntity.begin();
	std::advance(spot, spotNum);
	return spot->getCountur();
}

unsigned Camera::getID()
{
	return this->number;
}
