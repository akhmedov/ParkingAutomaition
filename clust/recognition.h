#pragma RECOGNITION_H

/********************************************************************
*			Akhmedov Rolan -- rolan.kharkiv@gmail.com				*
*					18 May 2016 -- version 0.9						*
* 	CV approach is based on k-means clustering and ... conturing	*
*		Open source lib is in use: OpenCV (GNU Licence GPLv2)		*
********************************************************************/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "imgutils.h"

class Recognition {
public:
	Recognition(short unsigned conturThresh = 200);
	Conturs buildConturs(cv::Mat image);
	cv::Mat buildCluster(cv::Mat image);
	Conturs findColorBlot(cv::Mat img, cv::Vec3b color);
		
	void setClusteringAttemps(short unsigned val);
	short unsigned getClusteringAttemps();
	
	void setClusteringIterations(unsigned val);
	unsigned getClusteringIterations();
	
	void setClusteringEps(double val);
	double getClusteringEps();

private:
	const static short unsigned clusterNum = 2;
	short unsigned conturThresh;
	short unsigned clusteringAttemps;
	unsigned clusteringIterations;
	double clusteringEps;
};
