#include "imgutils.h"

cv::Mat ImgUtils::minimize(cv::Mat src, int scale)
{
	int height = src.size().height / scale;
	int width = src.size().width / scale;
	cv::Mat res(height, width, src.type());
	cv::resize(src, res, res.size(), cv::INTER_LINEAR);
	return res;
}

cv::Mat ImgUtils::drawConturs(Conturs contours)
{
	cv::RNG rng(12345); // seed
	cv::Mat drawing = cv::Mat::zeros( contours.edges.size(), CV_8UC3 ); // CV_BGR2GRAYCV8UC3 WARNING!!!
	for( int i = 0; i< contours.location.size(); i++ )
	{
		cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), 
			rng.uniform(0,255) );
		cv::drawContours( drawing, contours.location, i, color , 2, 8, 
			contours.hierarchy, 0, cv::Point() );
	}
	return drawing;
}

cv::Mat ImgUtils::drawConturs(Conturs contours, cv::Scalar color )
{
	cv::RNG rng(12345); // seed
	cv::Mat drawing = cv::Mat::zeros( contours.edges.size(), CV_8UC3 ); // CV_BGR2GRAYCV8UC3 WARNING!!!
	for( int i = 0; i< contours.location.size(); i++ )
	{
		cv::drawContours( drawing, contours.location, i, color , 2, 8, 
			contours.hierarchy, 0, cv::Point() );
	}
	return drawing;
}

void ImgUtils::drowSpotStatus(Camera cam1, cv::Mat *screen, int markerSize)
{
	// TODO: Not impplemented
}

cv::Point2f ImgUtils::getCounturCenter(std::vector<cv::Point> countur)
{
	cv::Point2f center;
	float radius;
	cv::Moments::minEnclosingCircle(countur, center, radius);
	return center;
}

bool ImgUtils::isPointInside(cv::Point dot, std::vector<cv::Point> countur)
{
	// http://stackoverflow.com/questions/6882222/use-cvpointpolygontest
	// TODO: Not impplemented
}
