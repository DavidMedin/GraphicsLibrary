#pragma once
#include <glm.hpp>
#include <stdlib.h>
#include "Maths.h"
#include "../../../Src/GraphicsLibrary.h"

class Node {
private:

public:
	Node();
	void AddChild(Node* child);
	std::list<Node*> children;
	Node* parent;
	string name;
	virtual void Update() = 0;
};



class Object : public Node {
private:

public:
	glm::vec3* translate;
	glm::vec3* scale;
	glm::mat4* modelMatrix;
	glm::quat* orien;
	Mesh* mesh;
	Object();
	Object(string path);
	Object(string path, Node* parent);
	//will want to add a variadic function to take in as many shaders as mesh objects
	void Draw(Shader* shad,Camera* cam);
	void Draw(Camera* cam);
	void Translate(vec3 vector);
	void Rotate(vec3 axis, float angle);
	void Update();
	void ImGuiUpdate();
	void UpdateModelMatrix();
};

class Light : Node {
private:

public:
	vec3 translate;
	vec3 color;
	float intensity;

	Light(vec3 color, float intensity);
	void Update();
	void ImGuiUpdate();
};

class Particle : public Object {
private:
public:
	string imagePath;
	Particle();
	Particle(string path);
};

void UpdateNodes();

