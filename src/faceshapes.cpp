#include "faceshapes.hpp"

namespace app
{

FaceShapes* FaceShapes::instance = nullptr;
dlib::frontal_face_detector FaceShapes::faceDetector;
dlib::shape_predictor FaceShapes::faceModel;

bool FaceShapes::bgrbvar = false;
bool FaceShapes::bgrhbvar = false;
bool FaceShapes::bgrhbbvar = false;
bool FaceShapes::graybvar = false;
bool FaceShapes::grayhbvar = false;
bool FaceShapes::grayhbbvar = false;
cv::Scalar FaceShapes::bgrcolval = {255, 0, 0};
cv::Scalar FaceShapes::bgrhcolval = {255, 0, 0};
cv::Scalar FaceShapes::bgrhbcolval = {255, 0, 0};
cv::Scalar FaceShapes::graycolval = {255, 0, 0};
cv::Scalar FaceShapes::grayhcolval = {255, 0, 0};
cv::Scalar FaceShapes::grayhbcolval = {255, 0, 0};


FaceShapes::FaceShapes()
{
}

FaceShapes::~FaceShapes()
{
}

FaceShapes* FaceShapes::getInstance() 
{
	if (instance == nullptr)
	{
		instance = new FaceShapes();
		instance->initialize();
	}
	return instance;
}

void FaceShapes::releaseInstance()
{
	if (instance != nullptr)
		delete instance;
	instance = nullptr;
}

bool FaceShapes::initialize()
{
	faceDetector = dlib::get_frontal_face_detector();
	dlib::deserialize("../../../data/shape_predictor_68_face_landmarks.dat") >> faceModel;
	return true;
}


void FaceShapes::detectFaceShape()
{
	// Normalize frame
	cv::flip(frame, frameBGR, 1);
	cv::cvtColor(frameBGR, frameRGB, cv::COLOR_BGR2RGB);
	
	// Work with original frame
	cv::resize(frameBGR, frameBGRResized, cv::Size(), 0.5, 0.5);
	cv::GaussianBlur(frameBGRResized, frameBGRResizedBlured, cv::Size( 5, 5 ), 0, 0 );
	
	// Detect and draw faces on original frame
	if (bgrbvar)
	{
		IplImage imgBGR = cvIplImage(frameBGR);
		dlib::cv_image<dlib::bgr_pixel> cimgBGR(&imgBGR);
		std::vector<dlib::rectangle> detectedFacesBGR = faceDetector(cimgBGR);
		std::vector<dlib::full_object_detection> faceShapesBGR;
		for (unsigned long i = 0; i < detectedFacesBGR.size(); ++i)
			faceShapesBGR.push_back(faceModel(cimgBGR, detectedFacesBGR[i]));
		if (!faceShapesBGR.empty())
			for (unsigned long i = 0; i < faceShapesBGR.size(); ++i)
				drawFaceShape(&frameRGB, faceShapesBGR[i], bgrcolval, 1);
	}
	
	// Detect and draw faces on resized original frame
	if (bgrhbvar)
	{
		IplImage imgBGRResized = cvIplImage(frameBGRResized);
		dlib::cv_image<dlib::bgr_pixel> cimgBGRResized(&imgBGRResized);
		std::vector<dlib::rectangle> detectedFacesBGRResized = faceDetector(cimgBGRResized);
		std::vector<dlib::full_object_detection> faceShapesBGRResized;
		for (unsigned long i = 0; i < detectedFacesBGRResized.size(); ++i)
			faceShapesBGRResized.push_back(faceModel(cimgBGRResized, detectedFacesBGRResized[i]));
		if (!faceShapesBGRResized.empty())
			for (unsigned long i = 0; i < faceShapesBGRResized.size(); ++i)
				drawFaceShape(&frameRGB, faceShapesBGRResized[i], bgrhcolval, 2);
	}
	
	// Detect and draw faces on resized and blured original frame
	if (bgrhbbvar)
	{
		IplImage imgBGRResizedBlured = cvIplImage(frameBGRResizedBlured);
		dlib::cv_image<dlib::bgr_pixel> cimgBGRResizedBlured(&imgBGRResizedBlured);
		std::vector<dlib::rectangle> detectedFacesBGRResizedBlured = faceDetector(cimgBGRResizedBlured);
		std::vector<dlib::full_object_detection> faceShapesBGRResizedBlured;
		for (unsigned long i = 0; i < detectedFacesBGRResizedBlured.size(); ++i)
			faceShapesBGRResizedBlured.push_back(faceModel(cimgBGRResizedBlured, detectedFacesBGRResizedBlured[i]));
		if (!faceShapesBGRResizedBlured.empty())
			for (unsigned long i = 0; i < faceShapesBGRResizedBlured.size(); ++i)
				drawFaceShape(&frameRGB, faceShapesBGRResizedBlured[i], bgrhbcolval, 2);
	}
	
	// Work with grayscaled frame 
	cv::cvtColor(frameBGR, frameGray, cv::COLOR_BGR2GRAY);
	cv::resize(frameGray, frameGrayResized, cv::Size(), 0.5, 0.5);
	cv::GaussianBlur(frameGrayResized, frameGrayResizedBlured, cv::Size( 5, 5 ), 0, 0 );
	
	// Detect and draw faces on grayscaled frame
	if (graybvar)
	{
		IplImage imgGray = cvIplImage(frameGray);
		dlib::cv_image<unsigned char> cimgGray(&imgGray);
		std::vector<dlib::rectangle> detectedFacesGray = faceDetector(cimgGray);
		std::vector<dlib::full_object_detection> faceShapesGray;
		for (unsigned long i = 0; i < detectedFacesGray.size(); ++i)
			faceShapesGray.push_back(faceModel(cimgGray, detectedFacesGray[i]));
		if (!faceShapesGray.empty())
			for (unsigned long i = 0; i < faceShapesGray.size(); ++i)
				drawFaceShape(&frameRGB, faceShapesGray[i], graycolval, 1);
	}
	
	// Detect and draw faces on resized grayscaled frame
	if (grayhbvar)
	{
		IplImage imgGrayResized = cvIplImage(frameGrayResized);
		dlib::cv_image<unsigned char> cimgGrayResized(&imgGrayResized);
		std::vector<dlib::rectangle> detectedFacesGrayResized = faceDetector(cimgGrayResized);
		std::vector<dlib::full_object_detection> faceShapesGrayResized;
		for (unsigned long i = 0; i < detectedFacesGrayResized.size(); ++i)
			faceShapesGrayResized.push_back(faceModel(cimgGrayResized, detectedFacesGrayResized[i]));
		if (!faceShapesGrayResized.empty())
			for (unsigned long i = 0; i < faceShapesGrayResized.size(); ++i)
				drawFaceShape(&frameRGB, faceShapesGrayResized[i], grayhcolval, 2);
	}
	
	// Detect and draw faces on resized and blured grayscaled frame
	if (grayhbbvar)
	{
		IplImage imgGrayResizedBlured = cvIplImage(frameGrayResizedBlured);
		dlib::cv_image<unsigned char> cimgGrayResizedBlured(&imgGrayResizedBlured);
		std::vector<dlib::rectangle> detectedFacesGrayResizedBlured = faceDetector(cimgGrayResizedBlured);
		std::vector<dlib::full_object_detection> faceShapesGrayResizedBlured;
		for (unsigned long i = 0; i < detectedFacesGrayResizedBlured.size(); ++i)
			faceShapesGrayResizedBlured.push_back(faceModel(cimgGrayResizedBlured, detectedFacesGrayResizedBlured[i]));
		if (!faceShapesGrayResizedBlured.empty())
			for (unsigned long i = 0; i < faceShapesGrayResizedBlured.size(); ++i)
				drawFaceShape(&frameRGB, faceShapesGrayResizedBlured[i], grayhbcolval, 2);
	}
	
	//cv::equalizeHist(frameGray, frameGrayEqualized);
	//cv::Canny(tframe, frameCannied, 10, 20, 7, true);
	//cv::erode(frame, tleftEye, kernel);
}

void FaceShapes::drawFaceShape(cv::Mat* frame, dlib::full_object_detection shape, cv::Scalar color, int offset)
{
		// Simple visualization of face detection
		for (int i = 0; i < 68; i++)
		{
			cv::circle(*frame, cvPoint(shape.part(i).x()*offset, shape.part(i).y()*offset), 2, color, -1);
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
