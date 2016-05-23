/********************************************************************
*			Akhmedov Rolan -- rolan.kharkiv@gmail.com				*
*					19 May 2016 -- version 0.9						*
* 	 				Single frame computer Vision					*
*				Graphical vizualization of spot status				*
********************************************************************/

#include "../parking/camera.h"
#include "../clust/imgutils.h"
#include "../clust/recognition.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

void updateSpotStatus(Camera *cam1, cv::Mat ctrScreen)
{
	std::vector<unsigned> spots = cam1->getSpotNumbers();
	std::cout << spots.size() << std::endl;
	for (auto s : spots)
	 	cam1->setSpotStatus(s, busy);
	cam1->setSpotStatus(10, vacant);
	cam1->setSpotStatus(14, vacant);
}

Camera hardcodedCamera(cv::Mat parkingMap)
{
	Recognition core = Recognition(510);

	// get spot configuration
	std::list<Spot> spotConfiguration;
	int R = 255, G = 255, B = 0;
	cv::Vec3b yellow = cv::Vec3b(B, G, R);	
	Conturs ctrSpot = core.findColorBlot(parkingMap, yellow);
	for (int i = 0; i < ctrSpot.location.size(); i++) {
		Spot sp((unsigned) i, ctrSpot.location[i], "Null");
		spotConfiguration.push_back(sp);
	}

	// get road
	std::vector<std::vector<cv::Point> > roadMask;
	R = 0; G = 255; B = 255; 
	cv::Vec3b blue = cv::Vec3b(B, G, R);
	Conturs ctrRoad = core.findColorBlot(parkingMap, blue);
	for (int i = 0; i < ctrRoad.location.size(); i++) {
		roadMask.push_back(ctrRoad.location[i]);
	}

	Camera cam1 = Camera(1, spotConfiguration, roadMask);
	return cam1;
}

int main(int argc, char* argv[])
{
	Recognition core = Recognition();
	cv::Mat map = cv::imread(argv[1]);
	map = ImgUtils::minimize(map, 2);
	Camera cam1 = hardcodedCamera(map);

	cv::Mat screen = cv::imread(argv[2]);
	screen = ImgUtils::minimize(screen, 2);
	Conturs ctr = core.buildConturs(screen);
	cv::Mat ctrScreen = ImgUtils::drawConturs(ctr);

	updateSpotStatus(&cam1, ctrScreen);
	ImgUtils::drowSpotStatus(cam1, &screen);

	cv::imshow( "Spots Status", screen );
	cv::waitKey(0);
	return 0;
}
