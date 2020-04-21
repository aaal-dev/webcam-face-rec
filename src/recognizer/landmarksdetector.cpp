#include "landmarksdetector.hpp"

LandmarksDetector::LandmarksDetector(const char *model_file_path)
{
	dlib::deserialize(model_file_path) >> face_model;
}

LandmarksDetector::~LandmarksDetector(){}

template <typename T>
std::vector<cv::Point> LandmarksDetector::detect_landmarks(cv::Mat frame, cv::Rect &face_rect)
{
	IplImage ipl_img = cvIplImage(frame);
	dlib::cv_image<T> img(&ipl_img);
	dlib::rectangle face_region = dlib::rectangle(
		face_rect.x, 
		face_rect.y, 
		face_rect.x+face_rect.width, 
		face_rect.y+face_rect.height
	);
	dlib::full_object_detection landmarks = face_model(img, face_region);
	
	std::vector<cv::Point> cv_landmarks;
	for(int i = 0; i < 68; i++)
		cv_landmarks.push_back(cv::Point(landmarks.part(i).x(), landmarks.part(i).y()));
	return cv_landmarks;
}