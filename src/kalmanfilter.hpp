#ifndef KALMANFILTER_HPP
#define KALMANFILTER_HPP

// OpenCV
#include <opencv2/video/tracking.hpp>

namespace app
{

class KalmanFilter
{
public:
	// Functions
	static KalmanFilter* getInstance();
	static void releaseInstance();
	void start();
	static void setFilter();
	static void setPositionOfPoint(cv::Point point);
	static void getPrediction();
	static void getCorrection();
	
	// Variables
	static cv::Point predictedPosition;
	static cv::Point actualPosition;
	static cv::Point correctedPosition;
	
private:
	KalmanFilter();
	~KalmanFilter();
	
	// Functions
	
	
	// Variables
	static KalmanFilter* instance;
	static cv::KalmanFilter* kalmanFilter;
	static cv::Mat predictedMat;
	static cv::Mat actualMat;
	static cv::Mat correctedMat;
};

}

#endif // KALMANFILTER_HPP
