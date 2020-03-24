#ifndef FACESHAPES_HPP
#define FACESHAPES_HPP

// OpenCV
#include <opencv2/core/mat.hpp>

// Inner classes
#include "kalmanfilter.hpp"

namespace app
{

struct shape
{
	cv::Point predictedPosition;
	cv::Point actualPosition;
	cv::Point correctedPosition;
};

class FaceShapes
{
public:
	// Functions
	static void drawFaceShape();
	
	
	// Variables
	shape cheekToCheekShape[];
	shape rightEyebrowShape[];
	shape leftEyebrowShape[];
	shape noseShape[];
	shape rightEyeShape[];
	shape leftEyeShape[];
	shape outerMouthShape[];
	shape innerMouthShape[];
	
	
private:
	FaceShapes();
	~FaceShapes();
	
	// Functions
	
	
	
	// Variables
	
	
	
};

}

#endif // FACESHAPES_HPP
