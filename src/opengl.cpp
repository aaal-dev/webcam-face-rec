#include "opengl.hpp"

namespace app
{

OpenGL* OpenGL::instance = nullptr;
GLuint* OpenGL::VAO = nullptr;
GLuint* OpenGL::VBO = nullptr;
GLuint* OpenGL::EBO = nullptr;
GLuint* OpenGL::webcamTexture = nullptr;
GLuint* OpenGL::modelTexture = nullptr;

OpenGL::OpenGL(){}

OpenGL::~OpenGL(){}

OpenGL* OpenGL::getInstance() 
{
	if (instance == nullptr)
		instance = new OpenGL();
	return instance;
}

void OpenGL::releaseInstance()
{
	if (instance != nullptr)
		delete instance;
	instance = nullptr;
}

bool OpenGL::initialize(GLFWglproc glfwProcAddress)
{
	return gladLoadGLLoader((GLADloadproc) glfwProcAddress);
}

void OpenGL::run()
{
	glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
	
	// Create and compile our GLSL program from the shaders
	Shader::getInstance()->loadShaders( "vertexshader", "fragmentshader" );
	
	static const GLfloat vertices[] = {
	// |     Position     ||  TexCoord  |
	   -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, // top left
		1.0f,  1.0f,  0.0f,  1.0f,  0.0f, // top right
	   -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, // below left
		1.0f, -1.0f,  0.0f,  1.0f,  1.0f  // below right 
	};
	// Set up index
	static const GLuint indices[] = {
		0, 1, 2, 2, 3, 0
	};
	
	VAO = new GLuint();
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);
	
	VBO = new GLuint();
	glGenBuffers(1, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	
	EBO = new GLuint();
	glGenBuffers(1, EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	webcamTexture = new GLuint();
	glGenTextures(1, webcamTexture);
	glBindTexture(GL_TEXTURE_2D, *webcamTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	modelTexture = new GLuint();
	glGenTextures(1, modelTexture);
	glBindTexture(GL_TEXTURE_2D, *modelTexture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
}

void OpenGL::draw()
{
	glBindTexture(GL_TEXTURE_2D, *webcamTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(Shader::getInstance()->getShaderID());
	glBindVertexArray(*VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void OpenGL::cleanup()
{
	// Cleanup VBO
	glDeleteBuffers(1, VBO);
	glDeleteBuffers(1, EBO);
	glDeleteVertexArrays(1, VAO);
	glDeleteProgram(Shader::getInstance()->getShaderID());
}

void OpenGL::changeViewport(int bX, int bY, int eX, int eY)
{
	glViewport(bX, bY, eX, eY);
}

}
