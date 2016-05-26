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
	MYSQL* conn;
	Recognition core;
	~Configuration();
};

void printHelp();
void printVersion();

void readCameraFromSQL(MYSQL* connection);
void writeCameraStatusToSQL(MYSQL *connection, Camera cam);
void updateCameraStatusFromSQL(MYSQL *connection, Camera *cam);

Camera getCameraFromParkingMap(cv::Mat parkingMap);

int main(int argc, char* argv[])
{
	Configuration* conf = NULL;
	cv::Mat* map = NULL;

	for (int i = 1; i < argc; i++)
		if (argv[i] == std::string("--study"))
			map = new cv::Mat(cv::imread(argv[i+1]));
		else if (argv[i] == std::string("--config"))
			conf = new Configuration(argv[i+1]);
		else if (argv[i] == std::string("--help"))
			printHelp();
		else if (argv[i] == std::string("--version"))
			printVersion();

	if (conf != NULL) {

		// TODO: create mysql connection
		
		if (map != NULL) {

			// TODO: study
			std::cout << "studying..." << std::endl;
			Camera cam1 = getCameraFromParkingMap(*map);
			writeCameraStatusToSQL(conf->conn, cam1);

		} else {

			// TODO: recognition

			/*Camera cam1 = */readCameraFromSQL(conf->conn);

			cv::Mat image;
			for(;;) {
				if(!conf->stream.read(image)) {
					std::cout << "No frame" << std::endl;
					cv::waitKey();
				}
				cv::imshow("Output Window", image);
				if(cv::waitKey(1) >= 0) break;
			}
		}
	}

	// TODO: server termination
	return 0;
}

void printVersion()
{
	std::cout << "Parking Automaition Server version 0.9" << std::endl
		<< "Designed by Rolan Akhmedov -- rolan.kharkiv@gmail.com" << std::endl
		<< "Source code requiers distribution under GPLv3 licence" 
		<< std::endl;
}

void printHelp()
{
	std::cout << "Usage: server [OPSION] [--config POSIX_PATH]" << std::endl
	<< "Available options and staitmens:" << std::endl
	<< "--help 					print this text" << std::endl
	<< "--version 				print software informaiton" << std::endl
	<< "--study [IMAGE]				self-study on image" << std::endl
	<< "--seed [SPOT_NUM] --screen [IMAGE]	set seed to the spot form screenshot" << std::endl
	<< "--config [POSIX_PATH] 			read configuration from a custom place"
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

/*		int query_state = mysql_query(connection, "SHOW COLUMNS FROM ParkingAutomation.Spot");
		MYSQL_RES* res = mysql_store_result(connection);
		MYSQL_ROW row;

		while ((row = mysql_fetch_row(res)) != NULL) {
			printf("%s \t %s\t %s\t %s\t\t %s\n", 
				row[0], row[1], row[2], row[3], row[4]);
		}*/

		this->stream.open(*cam_url);
		this->core = Recognition();
	}
}

Configuration::~Configuration()
{

}

void readCameraFromSQL(MYSQL* connection)
{
	std::list<Spot> spotConfiguration;

	std::string select = "SELECT * FROM ParkingAutomation.Spot " + 
		"JOIN ParkingAutomation.Contour ON SpotNum = Num;";
	mysql_query(connection, select.c_str());
	MYSQL_RES* res = mysql_store_result(connection);
	MYSQL_ROW row;

	std::vector<cv::Point> contur;
	unsigned spotNum;
	SpotStatus sts;
	unsigned seed;
	std::string gps;
	while ( (row = mysql_fetch_row(res)) {
		if (spotNum != row[0]) {
			spotNum = row[0];
			sts = row[3]-1;
			seed = row[2];
			gps = row[1];
		}

		cv::Point2f dot(row[7], row[8]);
		contur.push_back(dot);
	}

/*	while ( (rowSpot = mysql_fetch_row(resSpot)) ) {
		std::vector<cv::Point> contur;

		Spot sp(row[0], contur, row[1]);
		sp.setDefaultFill(row[2]);
		sp.setStatus(row[3]);
		spotConfiguration.push_back(sp);
	} */

	int columnsNum = mysql_num_fields(res);
	std::cout << columnsNum << std::endl;
}

void writeCameraStatusToSQL(MYSQL *connection, Camera cam)
{
	std::string values;
	std::string insert;
	std::string tname;

	unsigned dotCount = 0;

	// TODO: road entity

	for (auto spotNum : cam.getSpotNumbers())
	{
		tname = "Spot";
		values = '\'' + std::to_string(spotNum) + "\'," + 
				 '\'' + cam.getSpotGPS(spotNum) + "\'," +
				 '\'' + std::to_string(cam.getDefaultFill(spotNum)) + "\'," +
				 '\'' + std::to_string(1+cam.getSpotStatus(spotNum)) + '\'';
		
		insert = "INSERT INTO ParkingAutomation." + tname +  
			" VALUES ("  + values + ")";
		mysql_query(connection, insert.c_str());
		// fprintf(stderr, "%s\n", mysql_error(connection));

		tname = "Contour";
		for (auto dot : cam.getSpotContour(spotNum)) {
			values = std::to_string(spotNum+dotCount) + "," + 
				 '\'' + std::to_string(cam.getID()) + "\'," +
				 '\'' + std::to_string(dot.x) + "\'," +
				 '\'' + std::to_string(dot.y) + "\'," +
				 '\'' + std::to_string(spotNum) + "\'," +
				 "NULL";
			insert = "INSERT INTO ParkingAutomation." + tname +  
				" VALUES ("  + values + ")";
			mysql_query(connection, insert.c_str());
			// fprintf(stderr, "%s\n", mysql_error(connection));
			dotCount++;
		}
	}
}

Camera getCameraFromParkingMap(cv::Mat parkingMap)
{
	Recognition core = Recognition(510);

	// get spot configuration
	std::list<Spot> spotConfiguration;
	int R = 255, G = 255, B = 0;
	cv::Vec3b yellow = cv::Vec3b(B, G, R);	
	Conturs ctrSpot = core.findColorBlot(parkingMap, yellow);
	for (int i = 0; i < ctrSpot.location.size(); i++) {
		Spot sp((unsigned) i, ctrSpot.location[i], "Null");
		sp.setDefaultFill(0);
		sp.setStatus(unknown);
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
