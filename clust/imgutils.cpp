#include "imgutils.h"

cv::Mat ImgUtils::minimize(cv::Mat src, int scale)
{
	int height = src.size().height / scale;
	int width = src.size().width / scale;
	cv::Mat res(height, width, src.type());
	cv::resize(src, res, res.size(), cv::INTER_LINEAR);
	return res;
}

unsigned ImgUtils::culcFill(const cv::Mat &image, const std::vector<cv::Point> &contur)
{
	unsigned fill = 0;
	for( int y = 0; y < image.rows; y++ )
		for( int x = 0; x < image.cols; x++ )
			if ( image.at<cv::Vec3b>(y,x)[0] != 0 ||
				image.at<cv::Vec3b>(y,x)[1] != 0 ||
				image.at<cv::Vec3b>(y,x)[2] != 0 ) 
				fill++;
	return fill;
}

cv::Mat ImgUtils::drawConturs(Conturs contours)
{
	cv::RNG rng(12345); // seed
	cv::Mat drawing = cv::Mat::zeros( contours.edges.size(), CV_8UC3 ); // CV_BGR2GRAYCV8UC3 WARNING!!!
	for( int i = 0; i< contours.location.size(); i++ )
	{
		cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), 
			rng.uniform(0,255) );
		cv::drawContours( drawing, contours.location, i, color , 2, 8, 
			contours.hierarchy, 0, cv::Point() );
	}
	return drawing;
}

cv::Mat ImgUtils::drawConturs(Conturs contours, cv::Scalar color )
{
	cv::RNG rng(12345); // seed
	cv::Mat drawing = cv::Mat::zeros( contours.edges.size(), CV_8UC3 ); // CV_BGR2GRAYCV8UC3 WARNING!!!
	for( int i = 0; i< contours.location.size(); i++ )
	{
		cv::drawContours( drawing, contours.location, i, color , 2, 8, 
			contours.hierarchy, 0, cv::Point() );
	}
	return drawing;
}

void ImgUtils::drowSpotStatus(Camera cam1, cv::Mat *screen, int markerSize)
{
	cv::Point2f center;
	SpotStatus stat;
	std::vector<cv::Point> ctr;
	cv::Scalar markerColor;
	std::vector<unsigned> spotNums = cam1.getSpotNumbers();
	for(int num = 0; num < spotNums.size(); ++num ) {
		ctr = cam1.getSpotContour(spotNums[num]);
		stat = cam1.getSpotStatus(spotNums[num]);
		center = ImgUtils::getCounturCenter(ctr);
		switch (stat) {
			case vacant:
				markerColor = CV_RGB(0,255,0);
				break;
			case busy:
				markerColor = CV_RGB(255,0,0);
				break;
			case unknown:
				markerColor = CV_RGB(230,230,230);
				break;
			default:
				markerColor = CV_RGB(0,0,0);
				break;
		}
		cv::circle(*screen, center, markerSize, markerColor, -1);
		//add text
			std::string text = std::to_string(num);
			int thickness = 2;
			double fontScale = 0.5;
			cv::Scalar textColor = CV_RGB(0,0,0);
			int fontFace = cv::FONT_HERSHEY_SCRIPT_SIMPLEX;
			int baseline = 0;
			cv::Size font = cv::getTextSize(text, fontFace, fontScale, thickness, &baseline);
			baseline += thickness;
			putText(*screen, text, center-cv::Point2f(5,-6), fontFace, fontScale, cv::Scalar::all(0), thickness, 8);
	}
}

cv::Point2f ImgUtils::getCounturCenter(std::vector<cv::Point> countur)
{
	cv::Point2f center;
	float radius;
	cv::minEnclosingCircle(countur, center, radius);
	return center;
}

bool ImgUtils::isPointInside(cv::Point2f dot, std::vector<cv::Point> countur)
{
	double res = cv::pointPolygonTest(countur, dot, false);
	// res = 1 or -1)
	return (1 == (int) res);
}
