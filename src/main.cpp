//#include <iostream>

// GLAD
#include "glad/glad.h"

// GLFW
#include "GLFW/glfw3.h"

// OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

// dlib
#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

#define     PI  3.141592654

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    std::cout << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int main()
{
    
    cv::VideoCapture cap(0);
	if (!cap.isOpened()) {
		std::cerr << "Unable to connect to camera" << std::endl;
		return 1;
	}
	
	dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
	dlib::shape_predictor pose_model;
	dlib::deserialize("../../shape_predictor_68_face_landmarks.dat") >> pose_model;

    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
    glfwInit();
    
	// Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Face", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }
    
    float tex_t = 1.0f; 
    float tex_u = 1.0f;
    int tex_w = 2048;
    int tex_h = 2048;
    int internalGlDataType = GL_RGB;
	int glDataType = GL_RGB;
    int w, h;
    
    GLuint* myTex1 = new GLuint[1];
    
    glGenTextures(1, myTex1);
    
    glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, myTex1[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, internalGlDataType, tex_w, tex_h, 0, GL_BGR, GL_UNSIGNED_BYTE, 0);  // init to black...
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glDisable(GL_TEXTURE_2D);
    
    GLuint* myTex2 = new GLuint[1];
    
    glGenTextures(1, myTex2);
    
    glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, myTex2[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, internalGlDataType, tex_w, tex_h, 0, GL_BGR, GL_UNSIGNED_BYTE, 0);  // init to black...
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glDisable(GL_TEXTURE_2D);
    
    // Disable vertical sync (on cards that support it)
    glfwSwapInterval( 1 );
	
    // Main loop
    int frames = 0;
    double t0 = glfwGetTime();
    double t;
    double fps;
    char* titlestr = new char[255];
    float screenFov = 60.0f;
    float eyeX;
	float eyeY;
    float halfFov = PI * screenFov / 360.0;
	float theTan = tanf(halfFov);
    float dist = eyeY / theTan;
	float nearDist = dist / 10.0;	// near / far clip plane
	float farDist = dist * 10.0;
	float aspect;
    
    while (!glfwWindowShouldClose(window))
    {
        
        // Get time
        t = glfwGetTime();
        
        // Calculate and display FPS (frames per second)
        if( (t-t0) > 1.0 || frames == 0 )
        {
            fps = (double)frames / (t-t0);
            sprintf( titlestr, "videoInput Demo App (%.1f FPS)", fps );
            glfwSetWindowTitle(window, titlestr);
            t0 = t;
            frames = 0;
        }
        frames ++;
        
		// Grab a frame
		//cv::Mat temp;
		//if (!cap.read(temp)) {
		//	break;
		//}
		//IplImage ipl_img = cvIplImage(temp);
		//dlib::cv_image<dlib::bgr_pixel> cimg(&ipl_img);
		
		// Detect faces 
		//std::vector<dlib::rectangle> faces = detector(cimg);
		
		// Find the pose of each face.
		//std::vector<dlib::full_object_detection> shapes;
		//for (unsigned long i = 0; i < faces.size(); ++i)
		//	shapes.push_back(pose_model(cimg, faces[i]));
		
		if ( w > tex_w || h > tex_h) {
			printf("image data too big - bailing \n");
			return;
		}
		
		//compute new tex co-ords based on the ratio of data's w, h to texture w,h;
		tex_t = (float)w / (float)tex_w;
		tex_u = (float)h / (float)tex_h;
		
		// update the texture image:
		glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, myTex1[0]);
			glTexSubImage2D(GL_TEXTURE_2D,0,0,0,w,h,glDataType,GL_UNSIGNED_BYTE,data);	
		glDisable(GL_TEXTURE_2D);
		
		glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, myTex2[0]);
			glTexSubImage2D(GL_TEXTURE_2D,0,0,0,w,h,glDataType,GL_UNSIGNED_BYTE,data);	
		glDisable(GL_TEXTURE_2D);
		
        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
		
		// Get window size (may be different than the requested size)
		glfwGetWindowSize( &w, &h );
        h = h > 0 ? h : 1;
		
		// Define the viewport dimensions
        glViewport(0, 0, w, h);
		
		// Clear the colorbuffer
        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
		
        eyeX = (float)w / 2.0;
        eyeY = (float)h / 2.0;
        aspect = (float)w/(float)h;
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(screenFov, aspect, nearDist, farDist);
        gluLookAt(eyeX, eyeY, dist, eyeX, eyeY, 0.0, 0.0, 1.0, 0.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        glEnable(GL_TEXTURE_2D);
            // bind the texture
            glBindTexture( GL_TEXTURE_2D, myTex1[0] );
        
            float px0 = x  ;		// up to you to get the aspect ratio right
            float py0 = y  
            float px1 = x+w;
            float py1 = y+h;
        
            glBegin( GL_QUADS );
                glTexCoord2f(0.0,0.0);		glVertex3f(px0, py0,0);
                glTexCoord2f(tex_t,0.0);	glVertex3f(px1, py0,0);
                glTexCoord2f(tex_t,tex_u);	glVertex3f(px1, py1,0);
                glTexCoord2f(0.0,tex_u);	glVertex3f(px0, py1,0);
            glEnd();
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_2D);
            // bind the texture
            glBindTexture( GL_TEXTURE_2D, myTex2[0] );
        
            float px0 = x  ;		// up to you to get the aspect ratio right
            float py0 = y  
            float px1 = x+w;
            float py1 = y+h;
        
            glBegin( GL_QUADS );
                glTexCoord2f(0.0,0.0);		glVertex3f(px0, py0,0);
                glTexCoord2f(tex_t,0.0);	glVertex3f(px1, py0,0);
                glTexCoord2f(tex_t,tex_u);	glVertex3f(px1, py1,0);
                glTexCoord2f(0.0,tex_u);	glVertex3f(px0, py1,0);
            glEnd();
        glDisable(GL_TEXTURE_2D);

        // Swap the screen buffers
        glfwSwapBuffers(window);

    }

    // Terminates GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}