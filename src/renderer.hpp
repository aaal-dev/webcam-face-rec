#ifndef OPENGL_HPP
#define OPENGL_HPP

#include <vector>
#include <cstring>

// GLAD
#include "glad/glad.h"

// GLFW
#include <GLFW/glfw3.h>

// NanoGUI
#include <nanogui/nanogui.h>

// Inner classes
#include "renderer/mesh.hpp"
#include "renderer/shader.hpp"

namespace app
{

class Renderer
{
public:
	
	// Functions
	static Renderer* getInstance();
	static void releaseInstance();
	bool initialize(GLADloadproc glfwProcAddress);
	void draw();
	void cleanup();
	void changeViewport(int bX, int bY, int eX, int eY);
	
	// Variables
	static GLsizei _width;
	static GLsizei _height;
	static GLboolean* _data;
	
	
	
private:
	Renderer();
	~Renderer();
	
	// Functions
	
	
	
	// Variables
	static Renderer* instance;
	static Shader* shader;
	
	std::vector<Mesh> meshes;
	
	static GLuint* VAO;
	static GLuint* VBO;
	static GLuint* EBO;
	static GLuint* webcamTexture;
	static GLuint* modelTexture;
	

};

}

#endif // OPENGL_HPP
