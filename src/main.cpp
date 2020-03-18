
#include "app.hpp"

//#define PI 3.141592654

//cv::KalmanFilter* kalmanFilter[68];

int main() 
{   
	
//		// Measure speed
//		double currentTime = glfwGetTime();
//		nbFrames++;
//		if( currentTime - lastTime >= 1.0 ) {
//			sprintf( titlestr, "videoInput Demo App (%.1f ms)", 1000.0/double(nbFrames) );
//			glfwSetWindowTitle(window1, titlestr);
//			nbFrames = 0;
//			lastTime += 1.0;
//		}
	
	if (app::App::getInstance()->initialize() == GL_FALSE)
		return 1;
	app::App::getInstance()->run();
	return 0;
}