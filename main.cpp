/********************************************************************
*           Akhmedov Rolan -- rolan.kharkiv@gmail.com               *
*                   18 May 2016 -- version 0.9                      *
*           Video streamming with recogniton results mask           *
*                   Event Logger output to stdout                   *
********************************************************************/

#include "logics/datafactory.h"
#include "clust/recognition.h"
#include "parking/camera.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <mysql/mysql.h>
#include <iostream>
#include <fstream>
#include <thread>

struct Configuration {
	Configuration(std::string pathToFile);
	cv::VideoCapture stream;
	MYSQL* conn;
	Recognition core;
	~Configuration();
};

void printHelp();
void printVersion();

void videoOut();
void updateCamStatus();

std::string reduce(const std::string& str,
				   const std::string& fill = " ",
				   const std::string& whitespace = " \t");
std::string trim(const std::string& str,
				 const std::string& whitespace = " \t");

Configuration* CONF = NULL;
cv::Mat CAMERASCREAN;
Camera CAMERA;

int main(int argc, char* argv[])
{
	cv::Mat* map = NULL;
	unsigned spotNumber = -1;

	for (int i = 1; i < argc; i++)
		if (argv[i] == std::string("--study"))
			map = new cv::Mat(cv::imread(argv[i+1]));
		else if (argv[i] == std::string("--config"))
			CONF = new Configuration(argv[i+1]);
		else if (argv[i] == std::string("--help"))
			printHelp();
		else if (argv[i] == std::string("--version"))
			printVersion();
		else if (argv[i] == std::string("--seed-for"))
			spotNumber = std::atoi(argv[i+1]);

	if (CONF != NULL) {

		if (map != NULL) {

			std::cout << "studying..." << std::endl;
			Camera cam1 = DataFactory::getCameraFromParkingMap(*map);
			DataFactory::writeCameraStatusToSQL(CONF->conn, cam1);

		} else {
			
			if(CONF->stream.read(CAMERASCREAN)) {
				
				if (spotNumber != -1)
					DataFactory::updateSpotSeed(CONF->stream, CONF->conn, spotNumber);
						
				CAMERA = DataFactory::readCameraFromSQL(CONF->conn);
				CONF->stream.read(CAMERASCREAN); // is necessary?

				std::thread streeming (videoOut);
				std::thread recognition (updateCamStatus);
				streeming.join();
				recognition.join();
			} 
			else return -1;

		}
	}

	return 0;
}

void updateCamStatus()
{
	Conturs ctr = CONF->core.buildConturs(CAMERASCREAN);
	cv::Mat ctrScreen = ImgUtils::drawConturs(ctr);
	for (auto spot : CAMERA.getSpotNumbers()) {
	unsigned imgFill = ImgUtils::culcFill(ctrScreen, CAMERA.getSpotContour(spot));
	unsigned deffaultFill = CAMERA.getDefaultFill(spot);
	if (imgFill > deffaultFill)
		CAMERA.setSpotStatus(spot, busy);
	else
		CAMERA.setSpotStatus(spot, vacant);
	}
}

void videoOut()
{
	for(;;) {
		if(!CONF->stream.read(CAMERASCREAN)) {
			std::cout << "No frame" << std::endl;
			cv::waitKey();
		}
		ImgUtils::drowSpotStatus(CAMERA, &CAMERASCREAN);
		cv::imshow("Output Window", CAMERASCREAN);
		// if(cv::waitKey(1) >= 0) break;
	}
}

void printVersion()
{
	std::cout 
		<< "Parking Automaition Server version 0.9" << std::endl
		<< "Designed by Rolan Akhmedov -- rolan.kharkiv@gmail.com" << std::endl
		<< "Source code requiers distribution under GPLv3 licence" 
		<< std::endl;
}

void printHelp()
{
	std::cout << "Usage: server [OPSION] --config POSIX_PATH" << std::endl
	<< "Available options and staitmens:" << std::endl
	<< "--help			print this text" << std::endl
	<< "--version		print software informaiton" << std::endl
	<< "--study [IMAGE]		self-study on image" << std::endl
	<< "--seed-for [SPOT_NUM]	set seed to the spot form screenshot" << std::endl
	<< "--config [POSIX_PATH]	read configuration from a custom place"
	<< std::endl;
}

Configuration::Configuration(std::string pathToFile)
{
	std::ifstream file(pathToFile);
	std::string line;

	std::string* cam_url = NULL;
	std::string* sql_server = NULL;
	std::string* sql_user = NULL;
	std::string* sql_password = NULL;
	std::string* sql_database = NULL;

	while (std::getline(file, line))
	{
		// line = std::regex_replace(line, std::regex("^ +| +$|( ) +"), "$1");
		line = reduce(line);
		if (line.size() == 0) {
			line = line;
		} else if (line[0] == '#') {
			line = line;
		} else if (std::string::npos != (int) line.find("cam_url = ")) {
			cam_url = new std::string(line.substr(10));
		} else if (std::string::npos != (int) line.find("sql_server = ")) {
			sql_server = new std::string(line.substr(13)); 
		} else if (std::string::npos != (int) line.find("sql_user = ")) {
			sql_user = new std::string(line.substr(11)); 
		} else if (std::string::npos != (int) line.find("sql_password = ")) {
			sql_password = new std::string(line.substr(15)); 
		} else if (std::string::npos != (int) line.find("sql_database = ")) {
			sql_database = new std::string(line.substr(15)); 
		}
	}

	if (cam_url != NULL && sql_server != NULL && sql_user != NULL 
		&& sql_password != NULL && sql_database != NULL) {

		this->conn = mysql_init(NULL);
		if (!mysql_real_connect(this->conn, sql_server->c_str(),
				sql_user->c_str(), sql_password->c_str(), 
				sql_database->c_str(), 0, NULL, 0))
		{
			std::cerr << mysql_error(this->conn) << std::endl;
		}

		if( !this->stream.open(*cam_url) )
			std::cout << "Error opening video stream or file" << std::endl;

		// TODO: read from config
		this->core = Recognition();
	}
}

Configuration::~Configuration()
{

}

std::string trim(const std::string& str,
				 const std::string& whitespace)
{
	const auto strBegin = str.find_first_not_of(whitespace);
	if (strBegin == std::string::npos)
		return ""; // no content

	const auto strEnd = str.find_last_not_of(whitespace);
	const auto strRange = strEnd - strBegin + 1;

	return str.substr(strBegin, strRange);
}

std::string reduce(const std::string& str,
				   const std::string& fill,
				   const std::string& whitespace)
{
	// trim first
	auto result = trim(str, whitespace);

	// replace sub ranges
	auto beginSpace = result.find_first_of(whitespace);
	while (beginSpace != std::string::npos)
	{
		const auto endSpace = result.find_first_not_of(whitespace, beginSpace);
		const auto range = endSpace - beginSpace;

		result.replace(beginSpace, range, fill);

		const auto newStart = beginSpace + fill.length();
		beginSpace = result.find_first_of(whitespace, newStart);
	}

	return result;
}
