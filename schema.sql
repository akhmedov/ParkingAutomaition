CREATE DATABASE IF NOT EXISTS ParkingAutomation;
GRANT ALL PRIVILEGES ON `ParkingAutomation`.* TO 'cv'@'localhost' IDENTIFIED BY 'FKrse36Gh' WITH GRANT OPTION;

USE ParkingAutomation;

CREATE TABLE IF NOT EXISTS `Spot` (
	Num		INTEGER NOT NULL,
	GPS		VARCHAR(20),
	Seed	INTEGER,
	Status	ENUM('vacant', 'busy', 'unknown'),
	PRIMARY KEY (Num)
);

CREATE TABLE IF NOT EXISTS `Road` (
	Side 		INTEGER	NOT NULL,
	PRIMARY KEY (Side)
);
 
CREATE TABLE IF NOT EXISTS `Contour` (
	PointID		INTEGER(100) NOT NULL,
	CameraID	VARCHAR(20) NOT NULL,
	valX		INTEGER NOT NULL,
	valY		INTEGER NOT NULL,
	SpotNum		INTEGER,
	RoadSide	INTEGER,
	FOREIGN KEY (SpotNum) REFERENCES Spot(Num),
	FOREIGN KEY (RoadSide) REFERENCES Road(Side),
	PRIMARY KEY (PointID),
	CHECK (SpotNum IS NOT NULL OR RoadSide IS NOT NULL)
);
