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
	cv::Mat loadedImage = cv::imread(argv[1]);
	cv::Mat scaledImage = ImgUtils::minimize(loadedImage, 2);
	int R = 255, G = 255, B = 0;
	Recognition core = Recognition(510);
	cv::Vec3b yellow = cv::Vec3b(B, G, R);
	Conturs ctr = core.findColorBlot(scaledImage, yellow);
	cv::Mat mask = ImgUtils::drawConturs(ctr);
	cv::imshow( "Loaded Image", scaledImage );
	cv::imshow( "Mask", mask );

	cv::Mat edges = ctr.edges;
	unsigned count = 0;
	for( unsigned y = 0; y < edges.rows; y++ )
		for( unsigned x = 0; x < edges.cols; x++ )
			if (edges.at<bool>(y,x)) {
				std::cout << y << " : " << x << std::endl;
				count++;
			}
	std::cout << count << std::endl;

	std::vector<cv::Vec4i> hierarchy = ctr.hierarchy;

	
	cv::waitKey(0);
	return 0;
}
