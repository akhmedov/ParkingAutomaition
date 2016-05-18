#include "recognition.h"

Recognition::Recognition(short unsigned threshs /*=200*/)
{
	this->conturThresh = threshs;
	this->imgScale = 2;
	this->clusteringAttemps = 5;
	this->clusteringEps = 0.0001;
	this->clusteringIterations = 1000;
}

cv::Mat Recognition::buildConturs(cv::Mat image)
{
	cv::RNG rng(12345); // seed

	cv::cvtColor( image, image, CV_BGR2GRAY );
	cv::blur( image, image, cv::Size(3,3) );
	
	cv::Mat edges;
	cv::Canny( image, edges, conturThresh, conturThresh*2, 3 );
	
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours( edges, contours, hierarchy, CV_RETR_TREE, 
		CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

	cv::Mat drawing = cv::Mat::zeros( edges.size(), CV_8UC3 );
	for( int i = 0; i< contours.size(); i++ )
	{
		cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), 
			rng.uniform(0,255) );
		cv::drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, 
			cv::Point() );
	}

	return drawing;
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

void Recognition::setImgScale(short unsigned val)
{
	this->imgScale = val;
}

short unsigned Recognition::getImgScale()
{
	return this->imgScale;
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

cv::Mat minimize(cv::Mat src, int scale)
{
	cv::Mat res(src.size()/scale, src.type());
	cv::resize(src, res, res.size(), cv::INTER_LINEAR);
	return res;
}
