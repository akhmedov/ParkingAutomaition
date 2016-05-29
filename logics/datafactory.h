#include "../clust/recognition.h"
#include "../parking/camera.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <mysql/mysql.h>

class DataFactory {
public:
	static void updateSpotSeed(cv::VideoCapture stream,
		MYSQL* connection, unsigned spotNumber);
	static Camera readCameraFromSQL(MYSQL* connection);
	static void writeCameraStatusToSQL(MYSQL *connection, Camera cam);
	// static void updateCameraStatusFromSQL(MYSQL *connection, Camera *cam);
	static Camera getCameraFromParkingMap(cv::Mat parkingMap);
private:
	static SpotStatus spotStatusFromCstr(std::string str);
};
