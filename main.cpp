/********************************************************************
*           Akhmedov Rolan -- rolan.kharkiv@gmail.com               *
*                   18 May 2016 -- version 0.9                      *
*           Video streamming with recogniton results mask           *
*                   Event Logger output to stdout                   *
********************************************************************/

#include "clust/recognition.h"
#include "parking/camera.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <mysql/mysql.h>
#include <iostream>
#include <fstream>
#include <regex>

struct Configuration {
	Configuration(std::string pathToFile);
	cv::VideoCapture stream;
	// Camera parking;
	const Recognition core;
	~Configuration();
};

void printHelp();
void printVersion();

Camera readCameraFromSQL(MYSQL* connection);
void writeCameraStatusToSQL(MYSQL *connection, Camera *cam);
void updateCameraStatusFromSQL(MYSQL *connection, Camera *cam);

int main(int argc, char* argv[])
{
	Configuration* conf = NULL;
	for (int i = 1; i < argc; i++)
		if (argv[i] == std::string("--config"))
			conf = new Configuration(argv[i+1]);
		else if (argv[i] == std::string("--help"))
			printHelp();
		else if (argv[i] == std::string("--version"))
			printVersion();

	if (conf != NULL) {
		// TODO: server start
	}

	// TODO: server termination

	return 0;
}

void printVersion()
{
	std::cout << "Parking Automaition Server version 0.9" << std::endl
		<< "Designed by Rolan Akhmedov -- rolan.kharkiv@gmail.com" << std::endl
		<< "Source code requiers distribution under GPLv3 licence" << std::endl;
}

void printHelp()
{
	std::cout << "Usage: server [OPSION] [--config POSIX_PATH]" << std::endl
	<< "Available options and staitmens:" << std::endl
	<< "--help                  print this text" << std::endl
	<< "--version               print software informaiton" << std::endl
	<< "--config                read configuration from a custom place"
	<< std::endl << std::endl;
}

Configuration::Configuration(std::string pathToFile)
{
	std::ifstream file(pathToFile);
	std::string line; 
	while (std::getline(file, line))
	{
		std::string* cam_url = NULL;
		std::string* parking_map = NULL;
		std::string* sql_server = NULL;
		std::string* sql_user = NULL;
		std::string* sql_password = NULL;
		std::string* sql_database = NULL;

		// line = std::regex_replace(line, std::regex("^ +| +$|( ) +"), "$1");
		if (line[0] == '#') {
			std::getline(file, line);
		} else if (line.find("cam_url =")) {
			cam_url = new std::string(line.substr(8));
		} else if (line.find("parking_map =")) {
			parking_map = new std::string(line.substr(12)); 
		} else if (line.find("sql_server =")) {
			sql_server = new std::string(line.substr(11)); 
		} else if (line.find("sql_user =")) {
			sql_user = new std::string(line.substr(9)); 
		} else if (line.find("sql_password =")) {
			sql_password = new std::string(line.substr(13)); 
		} else if (line.find("sql_database =")) {
			sql_database = new std::string(line.substr(13)); 
		}

		if (cam_url != NULL && sql_server != NULL && sql_user != NULL 
			&& sql_password != NULL) {
			
			MYSQL* connection = mysql_init(NULL);
			if (!mysql_real_connect(connection, sql_server->c_str(),
					sql_user->c_str(), sql_password->c_str(), 
					sql_database->c_str(), 0, NULL, 0))
			{
				std::cerr << mysql_error(connection) << std::endl;
			}


			this->stream.open(*cam_url);
			if (parking_map != NULL) {
				// TODO: set
			}
			// TODO: camera object
		}
	}
}

Configuration::~Configuration()
{

}
