#include "clust/recognition.h"
#include "parking/camera.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

int main(int argc, char* argv[])
{
	// String configPath = String(argv[1]);

	// stack of methods
/*	cv::Mat loadedImage = cv::imread(argv[2]);
	Recognition core = Recognition();
	cv::Mat scaledImage = ImgUtils::minimize(loadedImage, 2);
	cv::Mat clusteredImage = core.buildCluster(scaledImage);
	Conturs ctr = core.buildConturs(scaledImage);
	cv::Mat conturedImage = ImgUtils::drawConturs(ctr);
	cv::imshow( "Loaded Image", scaledImage );
	cv::imshow( "Clustered Image", clusteredImage );
	cv::imshow( "Contured Image", conturedImage );
*/

	cv::Mat loadedImage = cv::imread(argv[2]);
	cv::Mat scaledImage = ImgUtils::minimize(loadedImage, 2);
	int R = 255, G = 255, B = 0;
	Recognition core = Recognition(510);
	cv::Vec3b yellow = cv::Vec3b(B, G, R);
	Conturs ctr = core.findColorBlot(scaledImage, yellow);
	cv::Mat mask = ImgUtils::drawConturs(ctr);
	cv::imshow( "Loaded Image", scaledImage );
	cv::imshow( "Mask", mask );
	
	cv::waitKey(0);
	return 0;
}
