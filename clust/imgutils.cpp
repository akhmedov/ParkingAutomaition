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
	
	cv::Point2f center;
	SpotStatus stat;
	std::vector<cv::Point> ctr;
	cv::Scalar markerColor;
	std::vector<unsigned> spotNums = cam1.getSpotNumbers();
	for(int num = 0; num < spotNums.size(); ++num ) {
		ctr = cam1.getSpotContour(spotNums[num]);
		stat = cam1.getSpotStatus(spotNums[num]);
		center = ImgUtils::getCounturCenter(ctr);
		switch (stat) {
			case vacant:
				markerColor = CV_RGB(0,255,0);
				break;
			case busy:
				markerColor = CV_RGB(255,0,0);
				break;
			case unknown:
				markerColor = CV_RGB(255,255,255);
				break;
			default:
				markerColor = CV_RGB(0,0,0);
				break;
		}
		cv::circle(*screen, center, markerSize, markerColor, -1);
	}
	
}

cv::Point2f ImgUtils::getCounturCenter(std::vector<cv::Point> countur)
{
	cv::Point2f center;
	float radius;
	cv::minEnclosingCircle(countur, center, radius);
	return center;
}

bool ImgUtils::isPointInside(cv::Point2f dot, std::vector<cv::Point> countur)
{
	// http://stackoverflow.com/questions/6882222/use-cvpointpolygontest
	// TODO: Not impplemented
	double res = cv::pointPolygonTest(countur, dot, false);
	// res = 1 or -1)
	return (1 == (int) res);
}
