#include "timer.hpp"

namespace app
{

Timer* Timer::instance = nullptr;
double Timer::startTime;
double Timer::numberOfTicks;
float Timer::speed;

Timer::Timer()
{
}

Timer::~Timer()
{
}

Timer* Timer::getInstance() 
{
	if (instance == nullptr)
		instance = new Timer();
	return instance;
}

void Timer::releaseInstance()
{
	if (instance != nullptr)
		delete instance;
	instance = nullptr;
}

void Timer::start() 
{
	startTime = glfwGetTime();
	numberOfTicks = 0;
	speed = 0;
}

float Timer::getSpeedOnMS()
{
	numberOfTicks++;
	double elastedTime = glfwGetTime() - startTime;
	if (elastedTime >= 1.0) 
	{
		speed = 1000.0/numberOfTicks;
		numberOfTicks = 0;
		startTime += 1.0;
	}
	
	return speed;
}

}