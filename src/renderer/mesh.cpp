#include "mesh.hpp"

Mesh::Mesh(){}


Mesh::~Mesh(){}

std::vector<Vertex> Mesh::CreateQuad(float x, float y, float z, float size, float textureID)
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
	v2.Position = { x + size, y - size, z };
	v2.TexCoords = { 1.0f, 1.0f };
	v2.TexId = textureID;
	
	Vertex v3;
	v3.Position = { x, y - size, z };
	v3.TexCoords = { 0.0f, 1.0f };
	v3.TexId = textureID;
	
	return { v0, v1, v2, v3 };
}

void Mesh::load_model(const char* path)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		throw;
	}
    for(unsigned int i = 0; i < scene->mNumMeshes; i++)
		process_mesh(scene->mMeshes[i]);
}

void Mesh::process_mesh(aiMesh *mesh)
{
	load_vertices(mesh);
	load_indices(mesh);
}

void Mesh::load_vertices(const aiMesh *mesh)
{
	for(unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		Vec3 vector;
		vector.x = mesh->mVertices[i].x*0.1+0.5;
		vector.y = mesh->mVertices[i].y*0.1+0.5;
		vector.z = mesh->mVertices[i].z*0.1;
		vertex.Position = vector;
		
		//vector.x = mesh->mNormals[i].x;
		//vector.y = mesh->mNormals[i].y;
		//vector.z = mesh->mNormals[i].z;
		//vertex.Normal = vector;
		
		if(mesh->mTextureCoords[0]) 
		{
			Vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		} else {
			vertex.TexCoords = { 0.0f, 0.0f };
		}
		this->vertices.push_back(vertex);
    }
}

void Mesh::load_indices(const aiMesh *mesh)
{
	std::vector<unsigned int> indices;
	
	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for(unsigned int j = 0; j < face.mNumIndices; j++)
			this->indices.push_back(face.mIndices[j]);
	}
}