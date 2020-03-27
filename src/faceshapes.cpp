#include "faceshapes.hpp"

namespace app
{

FaceShapes* FaceShapes::instance = nullptr;
dlib::frontal_face_detector FaceShapes::faceDetector;
dlib::shape_predictor FaceShapes::faceModel;

FaceShapes::FaceShapes()
{
}

FaceShapes::~FaceShapes()
{
}

FaceShapes* FaceShapes::getInstance() 
{
	if (instance == nullptr)
		instance = new FaceShapes();
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
	dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> faceModel;
	return true;
}


void FaceShapes::detectFaceShape()
{
	
	
	cv::flip(frame, frameBGR, 1);
	cv::cvtColor(frameBGR, frameRGB, cv::COLOR_BGR2RGB);
	
	// Work with original frame
	cv::resize(frameBGR, frameBGRResized, cv::Size(), 0.5, 0.5);
	cv::GaussianBlur(frameBGRResized, frameBGRResizedBlured, cv::Size( 5, 5 ), 0, 0 );
	
	IplImage imgBGR = cvIplImage(frameBGR);
	IplImage imgBGRResized = cvIplImage(frameBGRResized);
	IplImage imgBGRResizedBlured = cvIplImage(frameBGRResizedBlured);
	
	dlib::cv_image<dlib::bgr_pixel> cimgBGR(&imgBGR);
	dlib::cv_image<dlib::bgr_pixel> cimgBGRResized(&imgBGRResized);
	dlib::cv_image<dlib::bgr_pixel> cimgBGRResizedBlured(&imgBGRResizedBlured);
	
	std::vector<dlib::rectangle> detectedFacesBGR = faceDetector(cimgBGR);
	std::vector<dlib::rectangle> detectedFacesBGRResized = faceDetector(cimgBGRResized);
	std::vector<dlib::rectangle> detectedFacesBGRResizedBlured = faceDetector(cimgBGRResizedBlured);
	
	std::vector<dlib::full_object_detection> faceShapesBGR;
	std::vector<dlib::full_object_detection> faceShapesBGRResized;
	std::vector<dlib::full_object_detection> faceShapesBGRResizedBlured;
	
	for (unsigned long i = 0; i < detectedFacesBGR.size(); ++i)
		faceShapesBGR.push_back(faceModel(cimgBGR, detectedFacesBGR[i]));
	for (unsigned long i = 0; i < detectedFacesBGRResized.size(); ++i)
		faceShapesBGRResized.push_back(faceModel(cimgBGRResized, detectedFacesBGRResized[i]));
	for (unsigned long i = 0; i < detectedFacesBGRResizedBlured.size(); ++i)
		faceShapesBGRResizedBlured.push_back(faceModel(cimgBGRResizedBlured, detectedFacesBGRResizedBlured[i]));
	if (!faceShapesBGR.empty())
		for (unsigned long i = 0; i < faceShapesBGR.size(); ++i)
			drawFaceShape(&frameRGB, faceShapesBGR[i], cv::Scalar(255,0,0), 1);
	if (!faceShapesBGRResized.empty())
		for (unsigned long i = 0; i < faceShapesBGRResized.size(); ++i)
			drawFaceShape(&frameRGB, faceShapesBGRResized[i], cv::Scalar(255,100,100), 2);
	if (!faceShapesBGRResizedBlured.empty())
		for (unsigned long i = 0; i < faceShapesBGRResizedBlured.size(); ++i)
			drawFaceShape(&frameRGB, faceShapesBGRResizedBlured[i], cv::Scalar(255,200,200), 2);
	
	// Work with grayscaled frame 
	cv::cvtColor(frameBGR, frameGray, cv::COLOR_BGR2GRAY);
	cv::resize(frameGray, frameGrayResized, cv::Size(), 0.5, 0.5);
	cv::GaussianBlur(frameGrayResized, frameGrayResizedBlured, cv::Size( 5, 5 ), 0, 0 );
	
	IplImage imgGray = cvIplImage(frameGray);
	IplImage imgGrayResized = cvIplImage(frameGrayResized);
	IplImage imgGrayResizedBlured = cvIplImage(frameGrayResizedBlured);
	
	dlib::cv_image<unsigned char> cimgGray(&imgGray);
	dlib::cv_image<unsigned char> cimgGrayResized(&imgGrayResized);
	dlib::cv_image<unsigned char> cimgGrayResizedBlured(&imgGrayResizedBlured);
	
	std::vector<dlib::rectangle> detectedFacesGray = faceDetector(cimgGray);
	std::vector<dlib::rectangle> detectedFacesGrayResized = faceDetector(cimgGrayResized);
	std::vector<dlib::rectangle> detectedFacesGrayResizedBlured = faceDetector(cimgGrayResizedBlured);
	
	std::vector<dlib::full_object_detection> faceShapesGray;
	std::vector<dlib::full_object_detection> faceShapesGrayResized;
	std::vector<dlib::full_object_detection> faceShapesGrayResizedBlured;
	
	for (unsigned long i = 0; i < detectedFacesGray.size(); ++i)
		faceShapesGray.push_back(faceModel(cimgGray, detectedFacesGray[i]));
	for (unsigned long i = 0; i < detectedFacesGrayResized.size(); ++i)
		faceShapesGrayResized.push_back(faceModel(cimgGrayResized, detectedFacesGrayResized[i]));
	for (unsigned long i = 0; i < detectedFacesGrayResizedBlured.size(); ++i)
		faceShapesGrayResizedBlured.push_back(faceModel(cimgGrayResizedBlured, detectedFacesGrayResizedBlured[i]));
	
	if (!faceShapesGray.empty())
		for (unsigned long i = 0; i < faceShapesGray.size(); ++i)
			drawFaceShape(&frameRGB, faceShapesGray[i], cv::Scalar(0,0,255), 1);
	if (!faceShapesGrayResized.empty())
		for (unsigned long i = 0; i < faceShapesGrayResized.size(); ++i)
			drawFaceShape(&frameRGB, faceShapesGrayResized[i], cv::Scalar(100,100,255), 2);
	if (!faceShapesGrayResizedBlured.empty())
		for (unsigned long i = 0; i < faceShapesGrayResizedBlured.size(); ++i)
			drawFaceShape(&frameRGB, faceShapesGrayResizedBlured[i], cv::Scalar(200,200,255), 2);
	
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
