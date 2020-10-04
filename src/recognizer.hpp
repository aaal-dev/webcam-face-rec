#ifndef RECOGNIZER_HPP
#define RECOGNIZER_HPP

#include <thread>

// Inner classes
#include "recognizer/face.hpp"
#include "recognizer/facedetector.hpp"
#include "recognizer/landmarksdetector.hpp"
#include "recognizer/kalmanfilter.hpp"

namespace app {

struct shape {
	cv::Point predictedPosition;
	cv::Point actualPosition;
	cv::Point correctedPosition;
};

class Recognizer {
public:
	Recognizer ();
	~Recognizer ();
	
	
	// Variables
	cv::Mat frame; 
	
	cv::Mat frameBGR, frameBGRResized, frameBGRResizedBlured;
	cv::Mat frameGray, frameGrayBlured, frameGrayResized, frameGrayResizedBlured, frameGrayEqualized;
	cv::Mat frameRGB; 
	cv::Mat frameCannied, frameBilateraled;
	
	std::vector <shape> cheekToCheekShape;
	std::vector <shape> rightEyebrowShape;
	std::vector <shape> leftEyebrowShape;
	std::vector <shape> noseShape;
	std::vector <shape> rightEyeShape;
	std::vector <shape> leftEyeShape;
	std::vector <shape> outerMouthShape;
	std::vector <shape> innerMouthShape;
	
	std::map <unsigned int, bool> boolFromGui;
	std::map <unsigned int, cv::Scalar> colorFromGui;
	std::array <float,3> face_rotation;
	
	
	// Functions
	void startDetector ();
	void setFrame (cv::Mat frame);
	void detectFaces ();
	void drawFaceShape (cv::Mat &frame, Face face, cv::Scalar color, int offset);
	std::array <float,3> get_face_rotation ();
	
	
	
private:
	// Variables
	//std::vector<Face> faces;
	dlib::frontal_face_detector face_detector;
	dlib::shape_predictor face_model;
	FaceDetector faceDetector;
	
	
	// Functions
	
	
	
};

}

#endif // RECOGNIZER_HPP
