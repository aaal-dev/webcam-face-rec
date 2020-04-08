#ifndef OPENGL_HPP
#define OPENGL_HPP

// GLAD
#include "glad/glad.h"

// Include GLM
#include <glm/glm.hpp>

// GLFW
#include <GLFW/glfw3.h>

// Inner classes
#include "shader.hpp"

namespace app
{



class OpenGL
{
public:
	
	// Functions
	static OpenGL* getInstance();
	static void releaseInstance();
	static bool initialize(GLFWglproc gladloadproc);
	void run();
	void draw();
	void cleanup();
	void changeViewport(int bX, int bY, int eX, int eY);
	
	// Variables
	GLsizei width;
	GLsizei height;
	unsigned char* data;
	
private:
	OpenGL();
	~OpenGL();
	
	// Functions
	
	
	
	// Variables
	static OpenGL* instance;
	
	static GLuint* VAO;
	static GLuint* VBO;
	static GLuint* EBO;
	static GLuint* webcamTexture;
	static GLuint* modelTexture;
	

};

}

#endif // OPENGL_HPP
