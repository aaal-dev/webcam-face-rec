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
#include "logger.hpp"
#include "renderer/mesh.hpp"
#include "renderer/shader.hpp"

namespace app
{

class Renderer
{
public:
	
	// Variables
	static Logger* logger;
	
	static GLsizei _width;
	static GLsizei _height;
	static GLboolean* _data;
	
	
	// Functions
	static Renderer* get_instance();
	
	bool initialize_GL(GLADloadproc glfwProcAddress);
	void draw();
	void cleanup();
	void changeViewport(int bX, int bY, int eX, int eY);
	void setHeadModelTransformation(glm::mat4 transformation);
	void setDrawProperties(std::map<std::string, bool> boolFromGui);
	
	
private:
	Renderer();
	~Renderer();
	
	
	// Variables
	static Renderer* instance;
	
	static Shader* shader;
	
	std::vector<Mesh> meshes;
	
	static GLuint* VAO;
	static GLuint* VBO;
	static GLuint* EBO;
	static GLuint* webcamTexture;
	static GLuint* modelTexture;
	
	
	// Functions
	static void release_instance();
	
	Mesh setWebcamMesh();
	Mesh setHeadModelMesh();
	
	
};

}

#endif // OPENGL_HPP
