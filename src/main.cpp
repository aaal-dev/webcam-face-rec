//#include <iostream>

// GLAD
#include "glad/glad.h"

// GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>

// OpenCV
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

// dlib
#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>

#include "common/shader.hpp"

#define     PI  3.141592654

// Window dimensions
const GLuint DEFAULT_WIDTH = 800, DEFAULT_HEIGHT = 600;

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    std::cout << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}

int main() 
{   
	// Initialise GLFW
	if(!glfwInit())
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}
    
	// Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif 
    
	cv::VideoCapture* camera = new cv::VideoCapture(0);
	if (!camera->isOpened())
	{
		fprintf( stderr, "Unable to connect to camera.\n" );
        getchar();
		return 1;
	}
    
	dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
	dlib::shape_predictor pose_model;
	dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> pose_model;

    fprintf( stdout, "Starting GLFW context, OpenGL 3.3...\n" );
    
    window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Face", NULL, NULL);
    if (window == NULL) {
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        fprintf( stderr, "Failed to initialize OpenGL context.\n" );
        getchar();
        return -1;
    }

    glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
    
    // Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "vertexshader", "fragmentshader" );
    
    static const GLfloat vertices[] = {
    // |     Position     ||  TexCoord  |
       -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, // top left
        1.0f,  1.0f,  0.0f,  1.0f,  0.0f, // top right
       -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, // below left
        1.0f, -1.0f,  0.0f,  1.0f,  1.0f  // below right 
    };
    // Set up index
    static const GLuint indices[] = {
        0, 2, 1, 1, 2, 3
    };
    
    GLuint* VAO = new GLuint();
    glGenVertexArrays(1, VAO);
    glBindVertexArray(*VAO);
    
    GLuint* VBO = new GLuint();
	glGenBuffers(1, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    
    GLuint* EBO = new GLuint();
    glGenBuffers(1, EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    GLuint* texture = new GLuint();
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    // Main loop
    int nbFrames = 0;
    double lastTime = glfwGetTime();
    char* titlestr = new char[255];
    
    while (!glfwWindowShouldClose(window)) {
        
        // Measure speed
        double currentTime = glfwGetTime();
        nbFrames++;
        if( currentTime - lastTime >= 1.0 ) {
            sprintf( titlestr, "videoInput Demo App (%.1f ms)", 1000.0/double(nbFrames) );
            glfwSetWindowTitle(window, titlestr);
            nbFrames = 0;
            lastTime += 1.0;
        }
        
        cv::Mat frame;
        IplImage ipl_img;
        
		// Grab a frame
		*camera >> frame;
		//cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
		
		ipl_img = cvIplImage(frame);
		dlib::cv_image<dlib::rgb_pixel> cimg(&ipl_img);
		
		// Detect faces 
		std::vector<dlib::rectangle> faces = detector(cimg);
		
		// Find the pose of each face.
		std::vector<dlib::full_object_detection> shapes;
		for (unsigned long i = 0; i < faces.size(); ++i)
			shapes.push_back(pose_model(cimg, faces[i]));
        
        if (!shapes.empty())
        {    
            // Simple visualization of face detection
			//for (int i = 17; i < 68; i++)
            //{
            //    cv::circle(frame, cvPoint(shapes[0].part(i).x(), shapes[0].part(i).y()), 2, cv::Scalar(255, 255, 255), -1);
            //    cv::putText(frame, std::to_string(i), cvPoint(shapes[0].part(i).x()-5, shapes[0].part(i).y()-5), cv::FONT_HERSHEY_SIMPLEX, 0.25, cv::Scalar(255, 127, 127), 1);
            //}
            
			// Advanced visualization of face detection
			// Shape of face (parts 0-16)
			for (int i = 0; i < 16; i++)
            {
                cv::line(
                    frame, 
                    cvPoint(shapes[0].part(i).x(), shapes[0].part(i).y()), 
                    cvPoint(shapes[0].part(i+1).x(), shapes[0].part(i+1).y()), 
                    cv::Scalar(127, 127, 127), 1);
            }
            
			// Right eyebrow shape (parts 17-21)
			for (int i = 17; i < 21; i++)
            {
                cv::line(
                    frame, 
                    cvPoint(shapes[0].part(i).x(), shapes[0].part(i).y()), 
                    cvPoint(shapes[0].part(i+1).x(), shapes[0].part(i+1).y()), 
                    cv::Scalar(255, 255, 255), 1);
            }
            
			// Left eyebrow shape (parts 22-26)
			for (int i = 22; i < 26; i++)
            {
                cv::line(
                    frame, 
                    cvPoint(shapes[0].part(i).x(), shapes[0].part(i).y()), 
                    cvPoint(shapes[0].part(i+1).x(), shapes[0].part(i+1).y()), 
                    cv::Scalar(255, 255, 255), 1);
            }
            
			// Nose shape (parts 27-35)
			for (int i = 27; i < 35; i++)
            {
                cv::line(
                    frame, 
                    cvPoint(shapes[0].part(i).x(), shapes[0].part(i).y()), 
                    cvPoint(shapes[0].part(i+1).x(), shapes[0].part(i+1).y()), 
                    cv::Scalar(127, 127, 127), 1);
            }
            
			// Right eye shape (parts 36-41)
            long int rightEye_x_min = shapes[0].part(36).x(), 
                rightEye_x_max = shapes[0].part(36).x(), 
                rightEye_y_min = shapes[0].part(36).y(), 
                rightEye_y_max = shapes[0].part(36).y();
			for (int i = 36; i < 41; i++)
            {
                if ( i == 40) 
				{
					cv::line(
						frame, 
						cvPoint(shapes[0].part(i).x(), shapes[0].part(i).y()), 
						cvPoint(shapes[0].part(36).x(), shapes[0].part(36).y()), 
						cv::Scalar(0, 0, 255), 1);
				} else {
					cv::line(
						frame, 
						cvPoint(shapes[0].part(i).x(), shapes[0].part(i).y()), 
						cvPoint(shapes[0].part(i+1).x(), shapes[0].part(i+1).y()), 
						cv::Scalar(127, 200, 255), 1);
				}
                
                // Found min and max of x and y for locate right eye
                if (shapes[0].part(i).x() < rightEye_x_min)
                    rightEye_x_min = shapes[0].part(i).x();
                if (shapes[0].part(i).x() > rightEye_x_max)
                    rightEye_x_max = shapes[0].part(i).x();
                if (shapes[0].part(i).y() < rightEye_y_min)
                    rightEye_y_min = shapes[0].part(i).y();
                if (shapes[0].part(i).y() > rightEye_y_max)
                    rightEye_y_max = shapes[0].part(i).y();
            }
            
            // Crop an eye to new frame
            //cv::line(
            //        frame, 
			//		cvPoint(rightEye_x_min, rightEye_y_min), 
			//		cvPoint(rightEye_x_min, rightEye_y_max), 
			//		cv::Scalar(255, 255, 255), 1);
            //cv::line(
            //        frame, 
			//		cvPoint(rightEye_x_min, rightEye_y_max), 
			//		cvPoint(rightEye_x_max, rightEye_y_max), 
			//		cv::Scalar(255, 255, 255), 1);
            //cv::line(
            //        frame, 
			//		cvPoint(rightEye_x_max, rightEye_y_max), 
			//		cvPoint(rightEye_x_max, rightEye_y_min), 
			//		cv::Scalar(255, 255, 255), 1);
            //cv::line(
            //        frame, 
			//		cvPoint(rightEye_x_max, rightEye_y_min), 
			//		cvPoint(rightEye_x_min, rightEye_y_min), 
			//		cv::Scalar(255, 255, 255), 1);
            //cv::Mat rightEye = frame(cv::Rect(rightEye_x_min, rightEye_y_min, rightEye_x_max - rightEye_x_min, rightEye_y_max - rightEye_y_min));
            
            
			// Left eye shape (parts 42-47)
            long int leftEye_x_min = shapes[0].part(42).x(), 
                leftEye_x_max = shapes[0].part(42).x(), 
                leftEye_y_min = shapes[0].part(42).y(), 
                leftEye_y_max = shapes[0].part(42).y();
            std::vector<cv::Point> leftEye_vector(0);
			for (int i = 42; i < 47; i++)
            {
                if ( i == 46) 
				{
					cv::line(
						frame, 
						cvPoint(shapes[0].part(i).x(), shapes[0].part(i).y()), 
						cvPoint(shapes[0].part(42).x(), shapes[0].part(42).y()), 
						cv::Scalar(0, 255, 0), 1);
				} else {
					cv::line(
						frame, 
						cvPoint(shapes[0].part(i).x(), shapes[0].part(i).y()), 
						cvPoint(shapes[0].part(i+1).x(), shapes[0].part(i+1).y()), 
						cv::Scalar(127, 200, 255), 1);
				}
                if (shapes[0].part(i).x() < leftEye_x_min)
                    leftEye_x_min = shapes[0].part(i).x();
                if (shapes[0].part(i).x() > leftEye_x_max)
                    leftEye_x_max = shapes[0].part(i).x();
                if (shapes[0].part(i).y() < leftEye_y_min)
                    leftEye_y_min = shapes[0].part(i).y();
                if (shapes[0].part(i).y() > leftEye_y_max)
                    leftEye_y_max = shapes[0].part(i).y();
                leftEye_vector.push_back(cvPoint(shapes[0].part(i).x(), shapes[0].part(i).y()));
            }
            
            // Crop an eye to new frame
            //cv::line(
            //       frame, 
			//		cvPoint(leftEye_x_min, leftEye_y_min), 
			//		cvPoint(leftEye_x_min, leftEye_y_max), 
			//		cv::Scalar(0, 0, 255), 1);
			//cv::putText(
			//		frame, 
			//		std::to_string(leftEye_x_min) + ":" + std::to_string(leftEye_y_min), 
			//		cvPoint(leftEye_x_min, leftEye_y_min), 
			//		cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(0, 255, 0), 1);
			//cv::putText(
			//		frame, 
			//		std::to_string(leftEye_x_max) + ":" + std::to_string(leftEye_y_max), 
			//		cvPoint(leftEye_x_max, leftEye_y_max), 
			//		cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(0, 0, 255), 1);		
            //cv::line(
            //        frame, 
			//		cvPoint(leftEye_x_min, leftEye_y_max), 
			//		cvPoint(leftEye_x_max, leftEye_y_max), 
			//		cv::Scalar(255, 0, 0), 1);
            //cv::line(
            //        frame, 
			//		cvPoint(leftEye_x_max, leftEye_y_max), 
			//		cvPoint(leftEye_x_max, leftEye_y_min), 
			//		cv::Scalar(0, 255, 0), 1);
            //cv::line(
            //        frame, 
			//		cvPoint(leftEye_x_max, leftEye_y_min), 
			//		cvPoint(leftEye_x_min, leftEye_y_min), 
			//		cv::Scalar(255, 255, 255), 1);
            cv::Rect leftEye_ROI1(cv::Point(leftEye_x_min, leftEye_y_min), cv::Point(leftEye_x_max, leftEye_y_max));
            cv::Mat leftEye = frame(leftEye_ROI1);
            cv::Rect leftEye_ROI2 = cv::boundingRect(leftEye_vector);
            //cv::rectangle(frame,leftEye_ROI1,cv::Scalar(0, 255, 0));
            //cv::rectangle(frame,leftEye_ROI2,cv::Scalar(0, 0, 255));
            cv::Mat leftEye1 = frame(leftEye_ROI2);
            cv::Mat leftEye2;
            cv::resize(leftEye1, leftEye2, cv::Size(50,(((float)50)/leftEye1.cols) * leftEye1.rows));
            cv::Mat leftEye3(leftEye2.rows,leftEye2.cols,CV_64F);
            for (int y = 0; y < leftEye2.rows; ++y) 
            {
                const uchar *Mr = leftEye2.ptr<uchar>(y);
                double *Or = leftEye3.ptr<double>(y);
                Or[0] = Mr[1] - Mr[0];
                for (int x = 1; x < leftEye2.cols - 1; ++x) 
                {
                    Or[x] = (Mr[x+1] - Mr[x-1])/2.0;
                }
                Or[leftEye2.cols-1] = Mr[leftEye2.cols-1] - Mr[leftEye2.cols-2];
            }
            cv::Mat leftEye4 = leftEye2.t();
            cv::Mat leftEye5(leftEye4.rows,leftEye4.cols,CV_64F);
            for (int y = 0; y < leftEye4.rows; ++y) 
            {
                const uchar *Mr = leftEye4.ptr<uchar>(y);
                double *Or = leftEye5.ptr<double>(y);
                Or[0] = Mr[1] - Mr[0];
                for (int x = 1; x < leftEye4.cols - 1; ++x) 
                {
                    Or[x] = (Mr[x+1] - Mr[x-1])/2.0;
                }
                Or[leftEye4.cols-1] = Mr[leftEye4.cols-1] - Mr[leftEye4.cols-2];
            }
            cv::Mat leftEye6 = leftEye5.t();
            cv::Mat leftEye7(leftEye3.rows,leftEye3.cols,CV_64F);
            for (int y = 0; y < leftEye3.rows; ++y) 
            {
                const double *Xr = leftEye3.ptr<double>(y), *Yr = leftEye6.ptr<double>(y);
                double *Mr = leftEye7.ptr<double>(y);
                for (int x = 0; x < leftEye3.cols; ++x) 
                {
                    double gX = Xr[x], gY = Yr[x];
                    double magnitude = sqrt((gX * gX) + (gY * gY));
                    Mr[x] = magnitude;
                }
            }
            cv::Scalar stdMagnGrad, meanMagnGrad;
            cv::meanStdDev(leftEye7, meanMagnGrad, stdMagnGrad);
            double stdDev = stdMagnGrad[0] / sqrt(leftEye7.rows*leftEye7.cols);
            double gradientThresh = 50.0 * stdDev + meanMagnGrad[0];
            for (int y = 0; y < leftEye2.rows; ++y) 
            {
                double *Xr = leftEye3.ptr<double>(y), *Yr = leftEye6.ptr<double>(y);
                const double *Mr = leftEye7.ptr<double>(y);
                for (int x = 0; x < leftEye2.cols; ++x)
                {
                    double gX = Xr[x], gY = Yr[x];
                    double magnitude = Mr[x];
                    if (magnitude > gradientThresh) 
                    {
                        Xr[x] = gX/magnitude;
                        Yr[x] = gY/magnitude;
                    } else {
                        Xr[x] = 0.0;
                        Yr[x] = 0.0;
                    }
                }
            }
            
            cv::Mat weight;
            cv::GaussianBlur( leftEye2, weight, cv::Size( 5, 5 ), 0, 0 );
            for (int y = 0; y < weight.rows; ++y) 
            {
                unsigned char *row = weight.ptr<unsigned char>(y);
                for (int x = 0; x < weight.cols; ++x) 
                {
                    row[x] = (255 - row[x]);
                }
            }
            
            cv::Mat outSum = cv::Mat::zeros(leftEye2.rows,leftEye2.cols,CV_64F);
            for (int y = 0; y < weight.rows; ++y) 
            {
                const double *Xr = leftEye3.ptr<double>(y), *Yr = leftEye6.ptr<double>(y);
                for (int x = 0; x < weight.cols; ++x)
                {
                    double gX = Xr[x], gY = Yr[x];
                    if (gX == 0.0 && gY == 0.0) 
                    {
                        continue;
                    }
                    for (int cy = 0; cy < outSum.rows; ++cy) 
                    {
                        double *Or = outSum.ptr<double>(cy);
                        const unsigned char *Wr = weight.ptr<unsigned char>(cy);
                        for (int cx = 0; cx < outSum.cols; ++cx) 
                        {
                            if (x == cx && y == cy) 
                            {
                                continue;
                            }
                            // create a vector from the possible center to the gradient origin
                            double dx = x - cx;
                            double dy = y - cy;
                            // normalize d
                            double magnitude = sqrt((dx * dx) + (dy * dy));
                            dx = dx / magnitude;
                            dy = dy / magnitude;
                            double dotProduct = dx*gX + dy*gY;
                            dotProduct = cv::max(0.0,dotProduct);
                            // square and multiply by the weight
                            if (true) 
                            {
                                Or[cx] += dotProduct * dotProduct * (Wr[cx]);
                            } else {
                                Or[cx] += dotProduct * dotProduct;
                            }
                        }
                    }
                }
            }
            double numGradients = (weight.rows*weight.cols);
            cv::Mat out;
            outSum.convertTo(out, CV_32F,1.0/numGradients);
            cv::Point maxP;
            double maxVal;
            cv::minMaxLoc(out, NULL,&maxVal,NULL,&maxP);
            cv::Mat floodClone;
            double floodThresh = maxVal * 0.97;
            cv::threshold(out, floodClone, floodThresh, 0.0f, cv::THRESH_TOZERO);
            cv::rectangle(floodClone,cv::Rect(0,0,floodClone.cols,floodClone.rows),255);
            cv::Mat mask(floodClone.rows, floodClone.cols, CV_8U, 255);
            std::queue<cv::Point> toDo;
            toDo.push(cv::Point(0,0));
            while (!toDo.empty()) 
            {
                cv::Point p = toDo.front();
                toDo.pop();
                if (floodClone.at<float>(p) == 0.0f)
                {
                    continue;
                }
                // add in every direction
                cv::Point np(p.x + 1, p.y); // right
                if (np.x >= 0 && np.x < floodClone.cols && np.y >= 0 && np.y < floodClone.rows) 
                    toDo.push(np);
                np.x = p.x - 1; np.y = p.y; // left
                if (np.x >= 0 && np.x < floodClone.cols && np.y >= 0 && np.y < floodClone.rows) 
                    toDo.push(np);
                np.x = p.x; np.y = p.y + 1; // down
                if (np.x >= 0 && np.x < floodClone.cols && np.y >= 0 && np.y < floodClone.rows) 
                    toDo.push(np);
                np.x = p.x; np.y = p.y - 1; // up
                if (np.x >= 0 && np.x < floodClone.cols && np.y >= 0 && np.y < floodClone.rows) 
                    toDo.push(np);
                // kill it
                floodClone.at<float>(p) = 0.0f;
                mask.at<uchar>(p) = 0;
            }
            cv::minMaxLoc(out, NULL,&maxVal,NULL,&maxP,mask);
            float ratio = (((float)(50))/leftEye_ROI2.width);
            cv::Point leftEye_pupil(leftEye_x_min + round(maxP.x / ratio), leftEye_y_min + round(maxP.y / ratio));
            
            cv::circle(frame, leftEye_pupil, 1, cv::Scalar(0,0,255),3);
            
			//cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7), cv::Point(3, 3));

            //double threshold = 0;
            //for (int threshold : {5, 100, 5})
            //{
            //    cv::Mat ttleftEye, tleftEye;
			//	cv::cvtColor(leftEye, ttleftEye, cv::COLOR_RGB2GRAY);
            //    cv::bilateralFilter(ttleftEye, tleftEye, 5, 100, 5, cv::BORDER_DEFAULT);
            //    cv::erode(tleftEye, tleftEye, kernel);
            //    cv::threshold(tleftEye, tleftEye, threshold, 255, cv::THRESH_BINARY);
            //    double tthreshold = ((tleftEye.cols * tleftEye.rows) - cv::countNonZero(tleftEye))/(tleftEye.cols * tleftEye.rows);
            //    if (tthreshold < threshold)
            //        threshold = tthreshold;
            //}
            //cv::Mat ttleftEye, tleftEye;
			//cv::cvtColor(leftEye, ttleftEye, cv::COLOR_RGB2GRAY);
            //cv::bilateralFilter(leftEye, tleftEye, 5, 100, 5, cv::BORDER_DEFAULT);
            //cv::erode(tleftEye, tleftEye, kernel);
            //cv::threshold(tleftEye, tleftEye, threshold, 255, cv::THRESH_BINARY);
            //std::vector< std::vector<cv::Point> > contours;
            //cv::findContours(tleftEye, contours, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);
            //cv::drawContours(frame, contours, -1, cv::Scalar(255, 255, 255), 1, 8, cv::noArray(), INT_MAX,cv::Point(leftEye_x_min,leftEye_y_min));
            
            
			// Outer mouth shape (parts 48-59)
			for (int i = 48; i < 59; i++)
            {
				if ( i == 58) 
				{
					cv::line(
						frame, 
						cvPoint(shapes[0].part(i).x(), shapes[0].part(i).y()), 
						cvPoint(shapes[0].part(48).x(), shapes[0].part(48).y()), 
						cv::Scalar(127, 127, 255), 1);
				} else {
					cv::line(		
						frame, 
						cvPoint(shapes[0].part(i).x(), shapes[0].part(i).y()), 
						cvPoint(shapes[0].part(i+1).x(), shapes[0].part(i+1).y()), 
						cv::Scalar(127, 127, 255), 1);
				}
            }
            
			// Inner mouth shape (parts 60-67)
			for (int i = 60; i < 68; i++)
            {
                if (i == 67) 
				{
					cv::line(
						frame, 
						cvPoint(shapes[0].part(i).x(), shapes[0].part(i).y()), 
						cvPoint(shapes[0].part(60).x(), shapes[0].part(60).y()), 
						cv::Scalar(127, 127, 255), 1);
				} else {
					cv::line(
						frame, 
						cvPoint(shapes[0].part(i).x(), shapes[0].part(i).y()), 
						cvPoint(shapes[0].part(i+1).x(), shapes[0].part(i+1).y()), 
						cv::Scalar(127, 127, 255), 1);
				}
            }
        }
        
		cv::flip(frame, frame, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, frame.data);
		//glfwSetWindowSize(window, frame.cols, frame.rows);
        
        
        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
		        
        // Use our shader
		glUseProgram(programID);
        
        // 1st attribute buffer : vertices
        glBindTexture(GL_TEXTURE_2D, *texture);
        glBindVertexArray(*VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

        // Swap the screen buffers
        glfwSwapBuffers(window);
        
        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
    }
    
    // Cleanup VBO
	glDeleteBuffers(1, VBO);
    glDeleteBuffers(1, EBO);
	glDeleteVertexArrays(1, VAO);
	glDeleteProgram(programID);
    
    camera->release();

    // Terminates GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}