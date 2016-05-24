/********************************************************************
*           Akhmedov Rolan -- rolan.kharkiv@gmail.com               *
*                   19 May 2016 -- version 0.9                      *
*                   OpenCV video proccessing IT                     *
*               Graphical vizualization of spot status              *
********************************************************************/

#include <stdio.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <iostream>

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

int main(int, char**) {

	Recognition core = Recognition();
	cv::Mat map = cv::imread(argv[1]);
	Camera cam1 = hardcodedCamera(map);

	cv::Mat screen = cv::imread(argv[2]);
	Conturs ctr = core.buildConturs(screen);
	cv::Mat ctrScreen = ImgUtils::drawConturs(ctr);

	hardcodeSpotStatus(&cam1, ctrScreen);
	ImgUtils::drowSpotStatus(cam1, &screen);

	cv::VideoCapture vcap;
	cv::Mat image;

	// This works on a D-Link CDS-932L
	const std::string videoStreamAddress = "rtsp://172.28.34.134/user=admin&password=&channel=1&stream=0.sdp?real_stream--rtp-caching=100";

	//open the video stream and make sure it's opened
	if(!vcap.open(videoStreamAddress)) {
		std::cout << "Error opening video stream or file" << std::endl;
		return -1;
	}

	for(;;) {
		if(!vcap.read(image)) {
			std::cout << "No frame" << std::endl;
			cv::waitKey();
		}
		cv::imshow("Output Window", image);
		if(cv::waitKey(1) >= 0) break;
	}

}
