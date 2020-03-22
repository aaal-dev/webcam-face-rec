#ifndef TIMER_HPP
#define TIMER_HPP

// GLFW
#include <GLFW/glfw3.h>

namespace app
{

class Timer
{
public:
	// Functions
	static Timer* getInstance();
	static void releaseInstance();
	void start();
	static float getSpeedOnMS();
	
	// Variables
	
	
private:
	Timer();
	~Timer();
	
	// Functions
	
	
	// Variables
	static Timer* instance;
	static double startTime;
	static double numberOfTicks;
	static float speed;
};

}

#endif // TIMER_HPP
