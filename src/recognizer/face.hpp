#ifndef FACE_HPP
#define FACE_HPP

#include <vector>

// OpenCV
#include <opencv2/opencv.hpp>

class Face
{
public:
	Face();
	~Face();
	
	// Variables
	cv::Mat frame;
	cv::Rect face_rect;
	std::vector<cv::Point> landmarks;
	cv::Point nose_base;
	cv::Point mouth_base;
	double theta;
	double tau;
};

#endif // FACE_HPP
