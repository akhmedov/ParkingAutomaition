#pragma once

/********************************************************************
*			Akhmedov Rolan -- rolan.kharkiv@gmail.com				*
*					19 May 2016 -- version 0.9						*
* 	CV approach is based on k-means clustering and ... conturing	*
*		Open source lib is in use: OpenCV (GNU Licence GPLv2)		*
********************************************************************/

enum SpotStatus {vacant, busy};

class Spot {
public:

private:
	
	unsigned number;
	bool trackable;
	SpotStatus status;
};
