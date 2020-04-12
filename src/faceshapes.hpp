#ifndef FACESHAPES_HPP
#define FACESHAPES_HPP

// dlib
#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>

// OpenCV
#include <opencv2/core/mat.hpp>
#include <opencv2/viz/types.hpp>

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
	static FaceShapes* getInstance();
	static void releaseInstance();
	static bool initialize();
	void detectFaceShape();
	static void drawFaceShape(cv::Mat* frame, dlib::full_object_detection shape, cv::Scalar color, int offset);
	
	
	// Variables
	cv::Mat frame; 
	
	cv::Mat frameBGR, frameBGRResized, frameBGRResizedBlured;
	cv::Mat frameGray, frameGrayBlured, frameGrayResized, frameGrayResizedBlured, frameGrayEqualized;
	cv::Mat frameRGB; 
	cv::Mat frameCannied, frameBilateraled;
	
	std::vector<shape> cheekToCheekShape;
	std::vector<shape> rightEyebrowShape;
	std::vector<shape> leftEyebrowShape;
	std::vector<shape> noseShape;
	std::vector<shape> rightEyeShape;
	std::vector<shape> leftEyeShape;
	std::vector<shape> outerMouthShape;
	std::vector<shape> innerMouthShape;
	
	static bool bgrbvar;
	static bool bgrhbvar;
	static bool bgrhbbvar;
	static bool graybvar;
	static bool grayhbvar;
	static bool grayhbbvar;
	static cv::Scalar bgrcolval;
	static cv::Scalar bgrhcolval;
	static cv::Scalar bgrhbcolval;
	static cv::Scalar graycolval;
	static cv::Scalar grayhcolval;
	static cv::Scalar grayhbcolval;
	
	
private:
	FaceShapes();
	~FaceShapes();
	
	// Functions
	
	
	
	// Variables
	static FaceShapes* instance;
	
	static dlib::frontal_face_detector faceDetector;
	static dlib::shape_predictor faceModel;
	
	
	
	
	
};

}

#endif // FACESHAPES_HPP
