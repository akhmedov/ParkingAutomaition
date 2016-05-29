#include "datafactory.h"

void DataFactory::updateSpotSeed(cv::VideoCapture stream,
	MYSQL* connection, unsigned spotNumber)
{
	unsigned newSeed;
	std::string select = std::string("UPDATE ParkingAutomation.Spot ") + 
		std::string("SET Seed=") + std::to_string(newSeed) + 
		std::string(" WHERE Num=") + std::to_string(spotNumber) +
		+ ";";
	mysql_query(connection, select.c_str());
}

Camera DataFactory::readCameraFromSQL(MYSQL* connection)
{
	std::list<Spot> spotConfiguration;

	std::string select = std::string("SELECT * FROM ParkingAutomation.Spot ") + 
		std::string("JOIN ParkingAutomation.Contour ON SpotNum = Num;");
	mysql_query(connection, select.c_str());
	MYSQL_RES* res = mysql_store_result(connection);
	MYSQL_ROW row;

	std::vector<cv::Point> contur;
	unsigned spotNum = -1;
	SpotStatus sts;
	unsigned seed;
	std::string gps;
	while ( (row = mysql_fetch_row(res)) ) {

		if (spotNum != std::atoi(row[0])) {
			if (!contur.empty()) {
				Spot sp(spotNum, contur, gps);
				sp.setDefaultFill(seed);
				sp.setStatus(sts);
				spotConfiguration.push_back(sp);
				contur.clear();
			}
			spotNum = std::atoi(row[0]);
			// MySQL enum numerating starts from 1 (not 0)
			sts = spotStatusFromCstr(row[3]);
			seed = std::atoi(row[2]);
			gps = std::atoi(row[1]);
		}

		cv::Point2f dot(std::atoi(row[6]), std::atoi(row[7]));
		contur.push_back(dot);
	}

	Spot sp(spotNum, contur, gps);
	sp.setDefaultFill(seed);
	sp.setStatus(sts);
	spotConfiguration.push_back(sp);
	contur.clear();

	// TODO: read road
	std::vector<std::vector<cv::Point> > roadMask;

	return 	Camera(1, spotConfiguration, roadMask);
}

void DataFactory::writeCameraStatusToSQL(MYSQL *connection, Camera cam)
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


Camera DataFactory::getCameraFromParkingMap(cv::Mat parkingMap)
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

SpotStatus DataFactory::spotStatusFromCstr(std::string str)
/* TODO: this hardcode must be refused by better 
implementaition of enum SpotStatus; */
{
	if (str == std::string("vacant")) 
		return vacant;
	else if (str == std::string("busy")) 
		return busy;
	else if (str == std::string("unknown")) 
		return unknown;
}
