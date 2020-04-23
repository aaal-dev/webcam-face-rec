#ifndef MESH_HPP
#define MESH_HPP

// GLM
#include <glm/glm.hpp>


// AssImp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Inner classes
#include "shader.hpp" 

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	float TexId;
};

class Mesh
{
public:
	Mesh();
	~Mesh();
	
	// Variables
	std::string name;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	glm::vec3 base_color;
	Shader shader;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	bool isDrawing;
	
	glm::mat4 normalization = glm::mat4(1.0);
	glm::mat4 transformation = glm::mat4(1.0);
	
	
	// Functions
	std::vector<Vertex> CreateQuad(float x, float y, float z, float size, float textureID);
	void load_model(const char* path);
	void process_mesh(aiMesh *mesh);
	void load_vertices(const aiMesh *mesh);
	void load_indices(const aiMesh *mesh);
};

#endif // MESH_HPP
