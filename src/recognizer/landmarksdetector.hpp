#ifndef LANDMARKSDETECTOR_HPP
#define LANDMARKSDETECTOR_HPP

#include <vector>

// dlib
#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>

// OpenCV
#include <opencv2/core/mat.hpp>

// Inner classes
#include "face.hpp"

class LandmarksDetector
{
public:
	LandmarksDetector(const char *model_file_path);
	~LandmarksDetector();
	
	template <typename T>
	std::vector<cv::Point> detect_landmarks(cv::Mat frame, cv::Rect &face_rect);
	
private:
	dlib::shape_predictor face_model;
	
	
	
};

#endif // LANDMARKSDETECTOR_HPP
