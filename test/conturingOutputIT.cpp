#include "../clust/imgutils.h"
#include "../clust/recognition.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

int main(int argc, char* argv[])
{

	int conturThresh = 471;
	cv::Mat edges;
	cv::Mat loadedImage = cv::imread(argv[1]);
	
	cv::cvtColor( loadedImage, loadedImage, CV_BGR2GRAY );
	cv::blur( loadedImage, loadedImage, cv::Size(3,3) );
	cv::Canny( loadedImage, edges, conturThresh, conturThresh*2, 3 );
	
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours( edges, contours, hierarchy, CV_RETR_EXTERNAL, 
		CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

	cv::RNG rng(12345); // seed
	cv::Mat drawing = cv::Mat::zeros( edges.size(), CV_8UC3 );
	for( int i = 0; i< contours.size(); i++ )
	{
		cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), 
			rng.uniform(0,255) );
		cv::drawContours( drawing, contours, i, color) /*, 2, 8, 
			contours.hierarchy, 0, cv::Point() )*/ ;
	}
	
	cv::waitKey(0);
	return 0;
}
