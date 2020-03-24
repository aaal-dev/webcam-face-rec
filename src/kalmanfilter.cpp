#include "kalmanfilter.hpp"

namespace app
{

KalmanFilter* KalmanFilter::instance = nullptr;
cv::KalmanFilter* KalmanFilter::kalmanFilter = nullptr;
cv::Point KalmanFilter::predictedPosition;
cv::Point KalmanFilter::actualPosition;
cv::Point KalmanFilter::correctedPosition;
cv::Mat KalmanFilter::predictedMat;
cv::Mat KalmanFilter::actualMat;
cv::Mat KalmanFilter::correctedMat;

KalmanFilter::KalmanFilter()
{
	kalmanFilter = new cv::KalmanFilter(4, 2, 0);
}
KalmanFilter::~KalmanFilter(){}

KalmanFilter* KalmanFilter::getInstance() 
{
	if (instance == nullptr)
		instance = new KalmanFilter();
	return instance;
}

void KalmanFilter::releaseInstance()
{
	if (instance != nullptr)
		delete instance;
	instance = nullptr;
}

void KalmanFilter::setFilter()
{
	float fltTransitionMatrixValues[4][4] = { { 1, 0, 1, 0 },
											  { 0, 1, 0, 1 },
											  { 0, 0, 1, 0 },
											  { 0, 0, 0, 1 } };
											  
	float fltMeasurementMatrixValues[2][4] = { { 1, 0, 0, 0 },
											   { 0, 1, 0, 0 } };
	kalmanFilter->transitionMatrix = cv::Mat(4, 4, CV_32F, fltTransitionMatrixValues);
	kalmanFilter->measurementMatrix = cv::Mat(2, 4, CV_32F, fltMeasurementMatrixValues);
	//cv::setIdentity(kalmanFilter->processNoiseCov, cv::Scalar::all(0.0001));
	//cv::setIdentity(kalmanFilter->measurementNoiseCov, cv::Scalar::all(10));
	//cv::setIdentity(kalmanFilter->errorCovPost, cv::Scalar::all(0.1));
	cv::setIdentity(kalmanFilter->processNoiseCov, cv::Scalar::all(1e-4));
	cv::setIdentity(kalmanFilter->measurementNoiseCov, cv::Scalar::all(1e-1));
	cv::setIdentity(kalmanFilter->errorCovPost, cv::Scalar::all(.1));
}

void KalmanFilter::setPositionOfPoint(cv::Point point)
{
	actualPosition = point;
	actualMat = cv::Mat(2, 1, CV_32F, cv::Scalar::all(0));
	actualMat.at<float>(0, 0) = actualPosition.x;
	actualMat.at<float>(1, 0) = actualPosition.y;
}

void KalmanFilter::getCorrection()
{
	predictedMat = kalmanFilter->predict();
	predictedPosition = cv::Point(predictedMat.at<float>(0), predictedMat.at<float>(1));
	correctedMat = kalmanFilter->correct(actualMat);
	correctedPosition = cv::Point(correctedMat.at<float>(0), correctedMat.at<float>(1));
}

}

