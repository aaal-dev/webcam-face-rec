//#include <iostream>

// GLAD
#include "glad/glad.h"

// GLFW
#include "GLFW/glfw3.h"
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>

// OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

// dlib
#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

#include <common/shader.hpp>

#define     PI  3.141592654

// Window dimensions
const GLuint DEFAULT_WIDTH = 800, DEFAULT_HEIGHT = 600;

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    std::cout << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
}

GLboolean* cvMat2TexInput(cv::Mat& img) {
    cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
    cv::flip(img, img, -1);
    return img.data;
}

int main() {    
    cv::VideoCapture* camera = new cv::VideoCapture(0);
	if (!camera->isOpened()) {
		fprintf( stderr, "Unable to connect to camera.\n");
        getchar();
		return 1;
	}
    cv::Mat frame;
    if (!camera->read(frame)) {
		fprintf( stderr, "Unable to get frame from camera.\n");
        getchar();
		return 1;
    }
    
    int width = frame.cols;
    int height = frame.rows;
    GLboolean* image = cvMat2TexInput(frame);
    IplImage ipl_img = cvIplImage(frame);
	
	dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
	dlib::shape_predictor pose_model;
	dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> pose_model;

    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
    
    // Initialise GLFW
	if(!glfwInit()) {
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}
    
	// Set all the required options for GLFW
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Face", NULL, NULL);
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
    
    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    
    // Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "vertexshader", "fragmentshader" );
    
    static const GLfloat vertices[] = {
        //     Position       TexCoord
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, // top left
        1.0f,   1.0f, 0.0f, 1.0f, 1.0f, // top right
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // below left
        1.0f,  -1.0f, 0.0f, 1.0f, 0.0f  // below right 
    };
    // Set up index
    static const GLuint indices[] = {
        0, 1, 2,
        1, 2, 3
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
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    GLuint* texture = new GLuint();
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
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
        
		// Grab a frame
		if (!camera->read(frame)) {
			break;
		}
		//ipl_img = cvIplImage(frame);
        
		//dlib::cv_image<dlib::bgr_pixel> cimg(&ipl_img);
		
		// Detect faces 
		//std::vector<dlib::rectangle> faces = detector(cimg);
		
		// Find the pose of each face.
		//std::vector<dlib::full_object_detection> shapes;
		//for (unsigned long i = 0; i < faces.size(); ++i)
		//	shapes.push_back(pose_model(cimg, faces[i]));
		
        image = cvMat2TexInput(frame);
        if(image) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, image);
        } else {
            fprintf( stderr, "Failed to load texture.\n" );
            getchar();
        }
        
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Use our shader
		glUseProgram(programID);
        
        // 1rst attribute buffer : vertices
		glBindTexture(GL_TEXTURE_2D, *texture);
        glBindVertexArray(*VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

    // Terminates GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}