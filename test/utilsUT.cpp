#include "../clust/imgutils.h"
#include "../clust/recognition.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
	// requires one contur image to stdin

	Recognition core = Recognition();
	cv::Mat screen = cv::imread(argv[1]);
	Conturs ctr = core.buildConturs(screen);
	cv::Mat ctrScreen = ImgUtils::drawConturs(ctr);

	cv::Point2f inside(50, 50);
	cv::Point2f outside(0, 0);

	bool one = ImgUtils::isPointInside(inside, ctr.location[0]);
	bool two = ImgUtils::isPointInside(outside, ctr.location[0]);
	
	std::cout << one << "  " << two << std::endl;

	cv::Point2f M00 = ImgUtils::getCounturCenter(ctr.location[0]);
	std::cout << "Circul center " << M00 << std::endl;

	cv::waitKey(0);
	return 0;
}
