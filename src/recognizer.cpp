#include "recognizer.hpp"

namespace app
{

Recognizer* Recognizer::instance = nullptr;
dlib::frontal_face_detector Recognizer::face_detector;
dlib::shape_predictor Recognizer::face_model;

bool Recognizer::bgrbvar = false;
bool Recognizer::bgrhbvar = false;
bool Recognizer::bgrhbbvar = false;
bool Recognizer::graybvar = false;
bool Recognizer::grayhbvar = false;
bool Recognizer::grayhbbvar = false;
cv::Scalar Recognizer::bgrcolval = {255, 0, 0};
cv::Scalar Recognizer::bgrhcolval = {255, 0, 0};
cv::Scalar Recognizer::bgrhbcolval = {255, 0, 0};
cv::Scalar Recognizer::graycolval = {255, 0, 0};
cv::Scalar Recognizer::grayhcolval = {255, 0, 0};
cv::Scalar Recognizer::grayhbcolval = {255, 0, 0};


Recognizer::Recognizer()
{
	faceDetector = FaceDetector();
}
Recognizer::~Recognizer(){}

Recognizer* Recognizer::getInstance() 
{
	if (instance == nullptr)
	{
		instance = new Recognizer();
	}
	return instance;
}

void Recognizer::releaseInstance()
{
	if (instance != nullptr)
		delete instance;
	instance = nullptr;
}

void Recognizer::detectFaces()
{
	// Normalize frame
	cv::flip(frame, frameBGR, 1);
	cv::cvtColor(frameBGR, frameRGB, cv::COLOR_BGR2RGB);
	
	// Work with original frame
	cv::resize(frameBGR, frameBGRResized, cv::Size(), 0.5, 0.5);
	cv::GaussianBlur(frameBGRResized, frameBGRResizedBlured, cv::Size( 5, 5 ), 0, 0 );
	
	// Work with grayscaled frame 
	cv::cvtColor(frameBGR, frameGray, cv::COLOR_BGR2GRAY);
	cv::resize(frameGray, frameGrayResized, cv::Size(), 0.5, 0.5);
	cv::GaussianBlur(frameGrayResized, frameGrayResizedBlured, cv::Size( 5, 5 ), 0, 0 );
	
	// Detect and draw faces on original frame
	if (bgrbvar)
	{
		std::vector<Face> faces = faceDetector.detect_faces<dlib::bgr_pixel>(frameBGR);
		for (Face &face : faces)
			drawFaceShape(frameRGB, face, bgrcolval, 1);
	}
	
	// Detect and draw faces on resized original frame
	if (bgrhbvar)
	{
		std::vector<Face> faces = faceDetector.detect_faces<dlib::bgr_pixel>(frameBGRResized);
		for (Face &face : faces)
			drawFaceShape(frameRGB, face, bgrhcolval, 2);
	}
	
	// Detect and draw faces on resized and blured original frame
	if (bgrhbbvar)
	{
		std::vector<Face> faces = faceDetector.detect_faces<dlib::bgr_pixel>(frameBGRResizedBlured);
		for (Face &face : faces)
			drawFaceShape(frameRGB, face, bgrhbcolval, 2);
	}
	
	// Detect and draw faces on grayscaled frame
	if (graybvar)
	{
		std::vector<Face> faces = faceDetector.detect_faces<unsigned char>(frameGray);
		for (Face &face : faces)
			drawFaceShape(frameRGB, face, graycolval, 1);
	}
	
	// Detect and draw faces on resized grayscaled frame
	if (grayhbvar)
	{
		std::vector<Face> faces = faceDetector.detect_faces<unsigned char>(frameGrayResized);
		for (Face &face : faces)
			drawFaceShape(frameRGB, face, grayhcolval, 2);
	}
	
	// Detect and draw faces on resized and blured grayscaled frame
	if (grayhbbvar)
	{
		std::vector<Face> faces = faceDetector.detect_faces<unsigned char>(frameGrayResizedBlured);
		for (Face &face : faces)
			drawFaceShape(frameRGB, face, grayhbcolval, 2);
	}
	
	//cv::equalizeHist(frameGray, frameGrayEqualized);
	//cv::Canny(tframe, frameCannied, 10, 20, 7, true);
	//cv::erode(frame, tleftEye, kernel);
}

void Recognizer::drawFaceShape(cv::Mat &frame, Face face, cv::Scalar color, int offset)
{
		// Simple visualization of face detection
		for (cv::Point landmark : face.landmarks)
		{
			cv::circle(frame, cvPoint(landmark.x*offset, landmark.y*offset), 2, color, -1);
		//	cv::circle(frame, correctedPosition[i], 2, cv::Scalar(255, 0, 0), -1);
		}
//		
//		// Advanced visualization of face detection
//		// Cheek to cheek shape (parts 0-16)
//		for (int i = 0; i < 16; i++)
//		{
//			if (drawActualPoints == true)
//				cv::line(frame, actualPosition[i], actualPosition[i+1], color, 1);
//			if (drawCorrectedPoints == true)
//				cv::line(frame, correctedPosition[i], correctedPosition[i+1], color, 1);
//		}
//		
//		// Right eyebrow shape (parts 17-21)
//		for (int i = 17; i < 21; i++)
//		{
//			if (drawActualPoints == true)
//				cv::line(frame, actualPosition[i], actualPosition[i+1], color, 1);
//			if (drawCorrectedPoints == true)
//				cv::line(frame, correctedPosition[i], correctedPosition[i+1], color, 1);
//		}
//		
//		// Left eyebrow shape (parts 22-26)
//		for (int i = 22; i < 26; i++)
//		{
//			if (drawActualPoints == true)
//				cv::line(frame, actualPosition[i], actualPosition[i+1], color, 1);
//			if (drawCorrectedPoints == true)
//				cv::line(frame, correctedPosition[i], correctedPosition[i+1], color, 1);
//		}
//		
//		// Nose shape (parts 27-35)
//		for (int i = 27; i < 35; i++)
//		{
//			if (drawActualPoints == true)
//				cv::line(frame, actualPosition[i], actualPosition[i+1], color, 1);
//			if (drawCorrectedPoints == true)
//				cv::line(frame, correctedPosition[i], correctedPosition[i+1], color, 1);
//		}
//		
//		cv::putText(
//				frame, 
//				std::to_string(shapes[0].part(35).x()*2 - shapes[0].part(31).x()*2), 
//				cvPoint(shapes[0].part(33).x()*2, shapes[0].part(33).y()*2), 
//				cv::FONT_HERSHEY_SIMPLEX, 0.4, color, 1);
//		
//		// Right eye shape (parts 36-41)
//		long int rightEye_x_min = actualPosition[36].x;
//		long int rightEye_x_max = actualPosition[41].x;
//		long int rightEye_y_min = actualPosition[36].y;
//		long int rightEye_y_max = actualPosition[41].y;
//		for (int i = 36; i < 41; i++)
//		{
//			if ( i == 40) 
//			{
//				if (drawActualPoints == true)
//					cv::line(frame, actualPosition[i], actualPosition[36], color, 1);
//				if (drawCorrectedPoints == true)
//					cv::line(frame, correctedPosition[i], correctedPosition[36], color, 1);
//			} else {
//				if (drawActualPoints == true)
//					cv::line(frame, actualPosition[i], actualPosition[i+1], color, 1);
//				if (drawCorrectedPoints == true)
//					cv::line(frame, correctedPosition[i], correctedPosition[i+1], color, 1);
//			}
//			
//			// Found min and max of x and y for locate right eye
//			if (actualPosition[i].x < rightEye_x_min)
//				rightEye_x_min = actualPosition[i].x;
//			if (actualPosition[i].x > rightEye_x_max)
//				rightEye_x_max = actualPosition[i].x;
//			if (actualPosition[i].y < rightEye_y_min)
//				rightEye_y_min = actualPosition[i].y;
//			if (actualPosition[i].y > rightEye_y_max)
//				rightEye_y_max = actualPosition[i].y;
//		}
//		
//		// Left eye shape (parts 42-47)
//		long int leftEye_x_min = actualPosition[42].x; 
//		long int leftEye_x_max = actualPosition[47].x; 
//		long int leftEye_y_min = actualPosition[42].y; 
//		long int leftEye_y_max = actualPosition[47].y;
//		for (int i = 42; i < 47; i++)
//		{
//			if ( i == 46) 
//			{
//				if (drawActualPoints == true)
//					cv::line(frame, actualPosition[i], actualPosition[42], color, 1);
//				if (drawCorrectedPoints == true)
//					cv::line(frame, correctedPosition[i], correctedPosition[42], color, 1);
//			} else {
//				if (drawActualPoints == true)
//					cv::line(frame, actualPosition[i], actualPosition[i+1], color, 1);
//				if (drawCorrectedPoints == true)
//					cv::line(frame, correctedPosition[i], correctedPosition[i+1], color, 1);
//			}
//			if (actualPosition[i].x < leftEye_x_min)
//				leftEye_x_min = actualPosition[i].x;
//			if (actualPosition[i].x > leftEye_x_max)
//				leftEye_x_max = actualPosition[i].x;
//			if (actualPosition[i].y < leftEye_y_min)
//				leftEye_y_min = actualPosition[i].y;
//			if (actualPosition[i].y > leftEye_y_max)
//				leftEye_y_max = actualPosition[i].y;
//		}
//		
//		// Outer mouth shape (parts 48-59)
//		for (int i = 48; i < 59; i++)
//		{
//			if ( i == 58) 
//			{
//				if (drawActualPoints == true)
//					cv::line(frame, actualPosition[i], actualPosition[48], color, 1);
//				if (drawCorrectedPoints == true)
//					cv::line(frame, correctedPosition[i], correctedPosition[48], color, 1);
//			} else {
//				if (drawActualPoints == true)
//					cv::line(frame, actualPosition[i], actualPosition[i+1], color, 1);
//				if (drawCorrectedPoints == true)
//					cv::line(frame, correctedPosition[i], correctedPosition[i+1], color, 1);
//			}
//		}
//		
//		// Inner mouth shape (parts 60-67)
//		for (int i = 60; i < 68; i++)
//		{
//			if (i == 67) 
//			{
//				if (drawActualPoints == true)
//					cv::line(frame, actualPosition[i], actualPosition[60], color, 1);
//				if (drawCorrectedPoints == true)
//					cv::line(frame, correctedPosition[i], correctedPosition[60], color, 1);
//			} else {
//				if (drawActualPoints == true)
//					cv::line(frame, actualPosition[i], actualPosition[i+1], color, 1);
//				if (drawCorrectedPoints == true)
//					cv::line(frame, correctedPosition[i], correctedPosition[i+1], color, 1);
//			}
//		}
}

}
