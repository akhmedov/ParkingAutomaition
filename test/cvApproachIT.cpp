/********************************************************************
*			Akhmedov Rolan -- rolan.kharkiv@gmail.com				*
*					19 May 2016 -- version 0.9						*
* 	 Intewgration test for Full stak of inmlemented CV methods		*
*				Visualization of recognition proccess				*
********************************************************************/

#include "../clust/imgutils.h"
#include "../clust/recognition.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

int main(int argc, char* argv[])
{
	cv::Mat loadedImage = cv::imread(argv[1]);
	cv::Mat scaledImage = ImgUtils::minimize(loadedImage, 2);
	Recognition core = Recognition();
	cv::Mat clusteredImage = core.buildCluster(scaledImage);
	// Conturs ctr = core.buildConturs(clusteredImage);
	Conturs ctr = core.buildConturs(scaledImage);
	cv::Mat conturedImage = ImgUtils::drawConturs(ctr);
	cv::imshow( "Loaded Image", scaledImage );
	cv::imshow( "Clustered Image", clusteredImage );
	cv::imshow( "Contured Image", conturedImage );
	
	cv::waitKey(0);
	return 0;
}
