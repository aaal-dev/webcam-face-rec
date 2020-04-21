#ifndef RECOGNIZER_HPP
#define RECOGNIZER_HPP

// Inner classes
#include "recognizer/face.hpp"
#include "recognizer/facedetector.hpp"
#include "recognizer/landmarksdetector.hpp"
#include "recognizer/kalmanfilter.hpp"

namespace app
{

struct shape
{
	cv::Point predictedPosition;
	cv::Point actualPosition;
	cv::Point correctedPosition;
};

class Recognizer
{
public:
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
	
	
	// Functions
	static Recognizer* getInstance();
	static void releaseInstance();
	void detectFaces();
	static void drawFaceShape(cv::Mat &frame, Face face, cv::Scalar color, int offset);
	
	
private:
	Recognizer();
	~Recognizer();
	
	// Variables
	static Recognizer* instance;
	//std::vector<Face> faces;
	static dlib::frontal_face_detector face_detector;
	static dlib::shape_predictor face_model;
	FaceDetector faceDetector;
	
	// Functions
	
	
	
};

}

#endif // RECOGNIZER_HPP
