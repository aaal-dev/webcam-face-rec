#include "kalmanfilter.hpp"

namespace app
{

KalmanFilter* KalmanFilter::instance = nullptr;

KalmanFilter::KalmanFilter()
{
}

KalmanFilter::~KalmanFilter()
{
}

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
	
}

void KalmanFilter::getCorrection()
{
	
}

}

