#pragma once
#ifdef GRAPHICSLIBRARY_EXPORTS
#define GRAPHICSLIBRARY_API __declspec(dllexport)
//assimp
#include <assImp/Importer.hpp>
#include <assImp/scene.h>
#include <assimp/postprocess.h>

//stb
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//glm
#include <glm.hpp>
#include <ext.hpp>
//glew
#include <glew.h>
using namespace glm;

#else
#define GRAPHICSLIBRARY_API __declspec(dllimport)
#endif
#include "Maths.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Error.h"
#include <vector>
#include "Camera.h"
#include "SceneTree.h"
#define DIFFUSE_SLOT 0
#define SPECULAR_SLOT 1


class GRAPHICSLIBRARY_API Texture : Attribute {
private:
	unsigned int openglID;
	unsigned int height, width;
	unsigned char* data;
	unsigned int slot;
public:
	Texture(unsigned int slot,unsigned char* data, unsigned int w, unsigned int h);
	Texture(unsigned int slot, std::string path);
	Texture();
	~Texture();
	void Bind(unsigned int slot = 0);
};

//needs to belong to a sceneNode, or a class or struct that contains a "list<void*> attributeList;" in it
class GRAPHICSLIBRARY_API Mesh : Attribute {
private:
	unsigned int indexCount; //called 'elements'
	unsigned int pointCount;
	VertexBuffer* vertexBuffer;
	VertexBuffer* textureUVBuffer;
	VertexBuffer* normalBuffer;
	VertexArray* VA;
	IndexBuffer* index;
	mat4* transform;

	//shouldn't belong to Mesh, rather be somewhere in the AttributeList
	Texture* texList[32];

	float* vertices;
	float* normals;
	float* textureUVs;
	unsigned int* indices;



public:
	//Mesh(float* data, unsigned int size, const unsigned int* indexData, unsigned int indexCount);
	Mesh(string path);
	void Draw(Camera* cam);
	void Draw(Shader* shad,Camera* cam);
};

