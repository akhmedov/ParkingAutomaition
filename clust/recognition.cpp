#include "recognition.h"

Recognition::Recognition(short unsigned threshs /*=200*/)
{
	this->conturThresh = threshs;
	this->clusteringAttemps = 5;
	this->clusteringEps = 0.0001;
	this->clusteringIterations = 1000;
}

Conturs Recognition::buildConturs(cv::Mat image)
{
	cv::cvtColor( image, image, CV_BGR2GRAY );
	cv::blur( image, image, cv::Size(3,3) );
	
	cv::Mat edges;
	cv::Canny( image, edges, conturThresh, conturThresh*2, 3 );
	
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours( edges, contours, hierarchy, CV_RETR_EXTERNAL, 
		CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

	return Conturs(edges, hierarchy, contours);
}

Conturs Recognition::findColorBlot(cv::Mat img, cv::Vec3b color)
{
	cv::Scalar black(0,0,0);
	cv::Vec3b wite = cv::Vec3b(255, 255, 255);
	cv::Mat mask = cv::Mat(img.size(), img.type(), black);
	for( int y = 0; y < img.rows; y++ )
		for( int x = 0; x < img.cols; x++ )
		{
			if ( img.at<cv::Vec3b>(y,x)[0] == color[0] &&
				img.at<cv::Vec3b>(y,x)[1] == color[1] &&
				img.at<cv::Vec3b>(y,x)[2] == color[2]
			) {
				mask.at<cv::Vec3b>(y,x) = wite;
			}
		}
	return this->buildConturs(mask);
}

cv::Mat Recognition::buildCluster(cv::Mat image)
{
	// convert input 3 channel image to 3D feacher space vectors
	cv::Mat samples(image.rows * image.cols, 3, CV_32F);
	for( int y = 0; y < image.rows; y++ )
		for( int x = 0; x < image.cols; x++ )
			for( int z = 0; z < 3; z++)
				samples.at<float>(y + x*image.rows, z) = 
					image.at<cv::Vec3b>(y,x)[z];

	cv::Mat labels, centers;
	cv::TermCriteria iterator = cv::TermCriteria(
		CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, clusteringIterations, clusteringEps);

	// pattern recognition
	cv::kmeans(samples, Recognition::clusterNum, labels, 
		iterator, clusteringAttemps, cv::KMEANS_PP_CENTERS, centers );

	// get to 3-channel image for exect cluster form 3D feacher space vectors
	cv::Mat layer( image.size(), image.type());
	for( int y = 0; y < image.rows; y++ )
		for( int x = 0; x < image.cols; x++ )
		{
			int cluster_idx = labels.at<int>(y + x*image.rows,0);
			if (true) {
				layer.at<cv::Vec3b>(y,x)[0] = centers.at<float>(cluster_idx, 0);
				layer.at<cv::Vec3b>(y,x)[1] = centers.at<float>(cluster_idx, 1);
				layer.at<cv::Vec3b>(y,x)[2] = centers.at<float>(cluster_idx, 2);
			} 
		}
	return layer;
}

void Recognition::setClusteringAttemps(short unsigned val)
{
	this->clusteringAttemps = val;
}

short unsigned Recognition::getClusteringAttemps()
{
	return this->clusteringAttemps;
}

void Recognition::setClusteringIterations(unsigned val)
{
	this->clusteringIterations = val;
}

unsigned Recognition::getClusteringIterations()
{
	return this->clusteringIterations;
}

void Recognition::setClusteringEps(double val)
{
	this->clusteringEps = val;
}

double Recognition::getClusteringEps()
{
	return this->clusteringEps;
}
