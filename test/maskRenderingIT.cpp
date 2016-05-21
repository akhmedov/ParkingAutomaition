/********************************************************************
*			Akhmedov Rolan -- rolan.kharkiv@gmail.com				*
*					19 May 2016 -- version 0.9						*
* 	 		Intewgration test for color mask analysis				*
*				Visualization of recognition proccess				*
********************************************************************/

#include "../clust/imgutils.h"
#include "../clust/recognition.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
	Recognition core = Recognition(510);
	cv::Mat loadedMap = cv::imread(argv[1]);
	cv::Mat loadedScreenSh = cv::imread(argv[2]);
	loadedScreenSh = ImgUtils::minimize(loadedScreenSh, 2);

	cv::Mat scaledMap = ImgUtils::minimize(loadedMap, 2);
	int R = 255, G = 255, B = 0;
	cv::Vec3b yellow = cv::Vec3b(B, G, R);	
	Conturs ctrSpot = core.findColorBlot(scaledMap, yellow);
	cv::Mat maskSpot = ImgUtils::drawConturs(ctrSpot, cv::Scalar(B,G,R));

	R = 0; G = 255; B = 255; 
	cv::Vec3b blue = cv::Vec3b(B, G, R);
	Conturs ctrRoad = core.findColorBlot(scaledMap, blue);
	cv::Mat maskRoad = ImgUtils::drawConturs(ctrRoad, cv::Scalar(B,G,R));

	cv::Mat mask = maskRoad + maskSpot;

	cv::imshow( "Loaded Map", scaledMap );
	cv::imshow( "Screenshot", loadedScreenSh );
	cv::imshow( "Mask", mask );
	std::cout << "Spot number: " << ctrSpot.location.size() << std::endl;

/*	cv::Mat edges = ctr.edges;
	cv::Mat example = cv::Mat::zeros(3, 3, CV_8UC1);
	std::cout << "edges type is CV_8U: " << (edges.type() == example.type()) << std::endl;
	std::cout << "edges has 1 channel: " << edges.channels() << std::endl;
	unsigned count = 0;
	for( int y = 0; y < edges.rows; y++ )
		for( int x = 0; x < edges.cols; x++ )
			if (edges.at<short>(y,x) != 0) {
				count++;
			}*/

	cv::waitKey(0);
	return 0;
}
