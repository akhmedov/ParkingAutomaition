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

#ifdef OUTFILES
	#define STDOUT(name, data) cv::imwrite(name, data)
#else
	#define STDOUT(name, data) cv::imshow(name, data)
#endif

void hardcodeSpotStatus(Camera *cam1, cv::Mat ctrScreen)
{
	std::vector<unsigned> spots = cam1->getSpotNumbers();
	for (auto s : spots)
	 	cam1->setSpotStatus(s, busy);
	cam1->setSpotStatus(14, vacant);
}

void updateSpotStatus(Camera &cam1, const cv::Mat &ctrScreen)
{
	for (auto spot : cam1.getSpotNumbers()) {
		unsigned imgFill = ImgUtils::culcFill(ctrScreen, cam1.getSpotContour(spot));
		unsigned deffaultFill = cam1.getDefaultFill(spot);
		if (imgFill > deffaultFill)
			cam1.setSpotStatus(spot, busy);
		else
			cam1.setSpotStatus(spot, vacant);
	}
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
		sp.setDefaultFill(68012);
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
	// map = ImgUtils::minimize(map, 2);
	Camera cam1 = hardcodedCamera(map);

	cv::Mat screen = cv::imread(argv[2]);
	// screen = ImgUtils::minimize(screen, 2);
	cv::Mat clustScreen = core.buildCluster(screen);
	Conturs ctr = core.buildConturs(screen);
	cv::Mat ctrScreen = ImgUtils::drawConturs(ctr);

	hardcodeSpotStatus(&cam1, ctrScreen);
	// updateSpotStatus(cam1, ctrScreen);
	ImgUtils::drowSpotStatus(cam1, &screen);
	
	// create ./out/ directory manualy !!!
	STDOUT("out/map.png", map);
	STDOUT("out/screen.png", cv::imread(argv[2]));
	STDOUT("out/cluster.png", clustScreen);
	STDOUT("out/contur.png", ctrScreen);
	STDOUT("out/result.png", screen);

	cv::waitKey(0);
	return 0;
}
