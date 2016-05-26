#include "spot.h"

Spot::Spot(unsigned num, std::vector<cv::Point> ctr, std::string gps)
{
	this->number = num;
	this->countur = ctr;
	this->gpsLocation = gps;
	this->trackable = true;
	this->status = unknown;
	this->defaultFill = 0;
}

void Spot::setDefaultFill(unsigned pixelCount)
{ 
	this->defaultFill = pixelCount;
}

void Spot::setStatus(SpotStatus sts)
{ 
	this->status = sts;
}

void Spot::track()
{ 
	this->trackable = true;
}

void Spot::untrack()
{
	this->trackable = false;
}

unsigned Spot::getDefaultFill()
{
	return this->defaultFill;
}

std::vector<cv::Point> Spot::getCountur()
{
	return this->countur;
}

unsigned Spot::getNumber()
{
	return this->number;
}

bool Spot::isTrackable()
{
	return trackable;
}

std::string Spot::getGPS()
{
	return gpsLocation;
}

SpotStatus Spot::getStatus()
{
	return status;
}
