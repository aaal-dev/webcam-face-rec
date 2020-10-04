#include "mesh.hpp"

namespace app {

Log* Mesh::logger = nullptr;

Mesh::Mesh(){indicesOffset = 0;}

Mesh::~Mesh(){}

std::vector<Vertex> Mesh::CreateQuad(float x, float y, float z, float size, float textureID)
{
	Vertex v0;
	v0.Position = { x, y, z };
	v0.TexCoords = { 0.0f, 0.0f };
	v0.TexId = textureID;
	
	Vertex v1;
	v1.Position = { x, y - size, z };
	v1.TexCoords = { 0.0f, 1.0f };
	v1.TexId = textureID;
	
	Vertex v2;
	v2.Position = { x + size, y - size, z };
	v2.TexCoords = { 1.0f, 1.0f };
	v2.TexId = textureID;
	
	Vertex v3;
	v3.Position = { x + size, y, z };
	v3.TexCoords = { 1.0f, 0.0f };
	v3.TexId = textureID;
	
	return { v0, v1, v2, v3 };
}

void Mesh::load_model(const char* path)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(path, 
		//aiProcess_JoinIdenticalVertices |
		aiProcess_Triangulate | 
		//aiProcess_FixInfacingNormals |
		aiProcess_SortByPType);
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		logger->logGLError("ERROR::ASSIMP:: %s\n", import.GetErrorString());
		throw;
	}
    for(unsigned int m = 0; m < scene->mNumMeshes; m++)
	{
		process_mesh(scene->mMeshes[m]);
		
		aiMaterial *material = scene->mMaterials[scene->mMeshes[m]->mMaterialIndex];
		aiColor3D color(0.f, 0.f, 0.f);
		material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		this->base_color = glm::vec3(color[0], color[1], color[2]);
	}
}

bool Mesh::load_shader(const char * vertex_file_path, const char * fragment_file_path) 
{
	shader = Shader();
	if (!shader.loadShaders(vertex_file_path, fragment_file_path)) {
		return false;
	}
	shaderID = shader.getShaderID();
	return true;
}

void Mesh::process_mesh(const aiMesh *mesh)
{
	load_vertices(mesh);
	load_indices(mesh);
	this->indicesOffset += mesh->mNumVertices;
}

void Mesh::process_face(const aiMesh *mesh, const aiFace *face)
{
	load_vertices(mesh, face);
	load_indices(face);
	
}

void Mesh::load_vertices(const aiMesh *mesh) 
{
	for(unsigned int v = 0; v < mesh->mNumVertices; v++)
	{
		Vertex vertex;
		vertex.Position.x = mesh->mVertices[v].x;
		vertex.Position.y = mesh->mVertices[v].y;
		vertex.Position.z = mesh->mVertices[v].z;
		
		vertex.Normal.x = mesh->mNormals[v].x;
		vertex.Normal.y = mesh->mNormals[v].y;
		vertex.Normal.z = mesh->mNormals[v].z;
		
		if(mesh->mTextureCoords[0]) 
		{
			vertex.TexCoords.x = mesh->mTextureCoords[0][v].x;
			vertex.TexCoords.y = mesh->mTextureCoords[0][v].y;
		} else {
			vertex.TexCoords = { 0.0f, 0.0f };
		}
		this->vertices.push_back(vertex);
    }
}

void Mesh::load_vertices(const aiMesh *mesh, const aiFace *face)
{
	for(unsigned int i = 0; i < face->mNumIndices; i++)
	{
		Vertex vertex;
		
		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;
		
		vertex.Normal.x = mesh->mNormals[i].x;
		vertex.Normal.y = mesh->mNormals[i].y;
		vertex.Normal.z = mesh->mNormals[i].z;
		
		if(mesh->mTextureCoords[0]) 
		{
			vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
		} else {
			vertex.TexCoords = { 0.0f, 0.0f };
		}
		this->vertices.push_back(vertex);
    }
}

void Mesh::load_indices(const aiMesh *mesh)
{
	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		if (face.mNumIndices < 3) 
			continue;
		assert(face.mNumIndices == 3);
		for(unsigned int j = 0; j < face.mNumIndices; j++)
			this->indices.push_back(face.mIndices[j] + indicesOffset);
	}
}

void Mesh::load_indices(const aiFace *face)
{
	for(unsigned int f = 0; f < face->mNumIndices; f++)
		this->indices.push_back(face->mIndices[f]);
}

}