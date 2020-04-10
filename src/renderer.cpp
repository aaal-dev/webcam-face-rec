#include "renderer.hpp"

namespace app
{

Renderer* Renderer::instance = nullptr;
GLuint* Renderer::VAO = nullptr;
GLuint* Renderer::VBO = nullptr;
GLuint* Renderer::EBO = nullptr;
GLuint* Renderer::webcamTexture = nullptr;
GLuint* Renderer::modelTexture = nullptr;
GLsizei Renderer::_width = 800;
GLsizei Renderer::_height = 600;
GLboolean* Renderer::_data = nullptr;

struct Vec2
{
	float x, y;
};

struct Vec3
{
	float x, y, z;
};

struct Vertex
{
	Vec3 Position;
	Vec2 TexCoords;
	float TexId;
};

Renderer::Renderer(){}

Renderer::~Renderer(){}

Renderer* Renderer::getInstance() 
{
	if (instance == nullptr)
		instance = new Renderer();
	return instance;
}

void Renderer::releaseInstance()
{
	if (instance != nullptr)
		delete instance;
	instance = nullptr;
}

bool Renderer::initialize(GLADloadproc glfwProcAddress)
{
	if (gladLoadGLLoader(glfwProcAddress))
	{
		glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
		
		// Create and compile our GLSL program from the shaders
		Shader::getInstance()->loadShaders( "shader.glsl.vertex", "shader.glsl.fragment" );
		auto loc = glGetUniformLocation(Shader::getInstance()->getShaderID(), "u_tex");
		int samplers[2] = { 0, 1 };
		glUniform1iv(loc, 2, samplers);
		
		VAO = new GLuint();
		glGenVertexArrays(1, VAO);
		glBindVertexArray(*VAO);
		
		VBO = new GLuint();
		glGenBuffers(1, VBO);
		glBindBuffer(GL_ARRAY_BUFFER, *VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 3, nullptr, GL_DYNAMIC_DRAW);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
		
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, TexCoords)));
		
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, TexId)));
		
		// Set up index
		static const GLuint indices[] = {
			0, 1, 2, 2, 3, 0
		};
		
		EBO = new GLuint();
		glGenBuffers(1, EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);
		
		webcamTexture = new GLuint();
		glGenTextures(1, webcamTexture);
		glBindTexture(GL_TEXTURE_2D, *webcamTexture);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		return true;
	}
	fprintf( stderr, "Failed to initialize OpenGL context.\n" );
	//getchar();
	return false;
}

std::array<Vertex, 4> CreateQuad(float x, float y, float z, float size, float textureID)
{
	Vertex v0;
	v0.Position = { x, y, z };
	v0.TexCoords = { 0.0f, 0.0f };
	v0.TexId = textureID;
	
	Vertex v1;
	v1.Position = { x + size, y, z };
	v1.TexCoords = { 1.0f, 0.0f };
	v1.TexId = textureID;
	
	Vertex v2;
	v2.Position = { x + size, y + size, z };
	v2.TexCoords = { 1.0f, 1.0f };
	v2.TexId = textureID;
	
	Vertex v3;
	v3.Position = { x, y + size, z };
	v3.TexCoords = { 0.0f, 1.0f };
	v3.TexId = textureID;
	
	return { v0, v1, v2, v3 };
}

void Renderer::draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, *webcamTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, _data);
	
	glUseProgram(Shader::getInstance()->getShaderID());
	glActiveTexture(GL_TEXTURE0);
	
	auto q0 = CreateQuad(-0.5f, -0.5f, 0.0f, 1.0f, 0.0f);
	Vertex vertices[3];
	memcpy(vertices, q0.data(), q0.size() * sizeof(Vertex));
	
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	
	glBindVertexArray(*VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Renderer::cleanup()
{
	// Cleanup VBO
	glDeleteBuffers(1, VBO);
	glDeleteBuffers(1, EBO);
	glDeleteVertexArrays(1, VAO);
	glDeleteProgram(Shader::getInstance()->getShaderID());
}

void Renderer::changeViewport(int bX, int bY, int eX, int eY)
{
	glViewport(bX, bY, eX, eY);
}

}
