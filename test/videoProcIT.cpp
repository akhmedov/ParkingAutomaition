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

int main(int, char**) {

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
