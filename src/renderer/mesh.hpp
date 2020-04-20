#ifndef MESH_HPP
#define MESH_HPP

// AssImp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


// Inner classes
#include "shader.hpp" 

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

class Mesh
{
public:
	Mesh();
	~Mesh();
	
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	Shader shader;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	
	std::vector<Vertex> CreateQuad(float x, float y, float z, float size, float textureID);
	void load_model(const char* path);
	void process_mesh(aiMesh *mesh);
	void load_vertices(const aiMesh *mesh);
	void load_indices(const aiMesh *mesh);
};

#endif // MESH_HPP
