#ifndef KALMANFILTER_HPP
#define KALMANFILTER_HPP

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
	static void getCorrection();
	// Variables
	
	
private:
	KalmanFilter();
	~KalmanFilter();
	
	// Functions
	
	
	// Variables
	static KalmanFilter* instance;
	
};

}

#endif // KALMANFILTER_HPP
