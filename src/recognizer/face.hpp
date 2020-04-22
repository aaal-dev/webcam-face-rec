#ifndef FACE_HPP
#define FACE_HPP

#include <vector>
#include <array>

// OpenCV
#include <opencv2/opencv.hpp>

// Inner classes
#include "kalmanfilter.hpp"

class Face
{
public:
	Face();
	~Face();
	
	// Variables
	cv::Mat frame;
	cv::Rect face_rect;
	std::vector<cv::Point> landmarks;
	std::vector<cv::Rect> eyes_rects;
	std::vector<cv::Rect> lefteye_rects;
	std::vector<cv::Rect> righteye_rects;
	cv::Point nose_base;
	cv::Point nose_tip;
	cv::Point mouth_base;
	double theta;
	double tau;
	std::array<float,3> rotation;
};

#endif // FACE_HPP
