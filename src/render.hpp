#ifndef OPENGL_HPP
#define OPENGL_HPP


#include <vector>
#include <cstring>
#include <map>

// GLAD
#include "glad/glad.h"

// GLM
#include <glm/gtc/matrix_transform.hpp>

// Inner classes
#include "log.hpp"
#include "statecon.hpp"
#include "renderer/mesh.hpp"

namespace app
{

struct CameraFrame {
	unsigned char* data;
	int width;
	int height;
};

class Render
{
public:
	Render();
	~Render();
	
	
	// Variables
	GLsizei width;
	GLsizei height;
	GLboolean* data;
	CameraFrame cameraFrame;
	
	
	// Functions
	bool initialize_GL(GLADloadproc glfwProcAddress);
	void drawCleanUp();
	void drawWebCamMesh();
	void draw();
	void cleanup();
	void changeViewport(int bX, int bY, int eX, int eY);
	void setHeadModelTransformation(glm::mat4 transformation);
	void setDrawProperties(std::map<std::string, bool> boolFromGui);
	
	
	
private:
	// Variables
	static Log* logger;
	static StateCon* stater;
	std::vector<Mesh> meshes;
	Mesh webcam;
	
	// Functions
	void logGLparams ();
	bool setWebcamMesh();
	Mesh setHeadModelMesh();
	
	
};

}

#endif // OPENGL_HPP
