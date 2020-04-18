#include "Bone.h"

#define GETMATRIX(i) skelly->boneMatrices[i->index]
#define GETOFFSET(i) skelly->boneOffsets[i->index]

void ConvertAssimpMatrix(glm::vec3* dest, aiMatrix4x4* m) {
	*dest = glm::vec3(m->a4, m->b4, m->c4);
	//return new mat4(
	//	1.0f, 0.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f, 0.0f,
	//	0.0f, 0.0f, 1.0f, 0.0f,
	//	m.a4, m.b4, m.c4, m.d4
	//);
}

unsigned int boneInstances = 0;
BoneNode::BoneNode()
{
	boneInstances++;
	name = "BoneNode_" + std::to_string(boneInstances);
	
	index = NULL;

	parent = nullptr;
}

BoneNode::BoneNode(Skeleton* skelly, aiNode* node,aiMesh* mesh)
{
	//node should be the root scene node
	//looks for a match for any of the given names
	kill = false;
	parent = nullptr;
	index = -1;

	this->skelly = skelly;
	bool hasBone=false;
	for (int i = 0; i < mesh->mNumBones; i++) {
		name = node->mName.C_Str();
		if (strcmp(mesh->mBones[i]->mName.C_Str(), node->mName.C_Str())==0) {//this node is one of our bones!
			hasBone = true;
			index = i;
			break;
		}
	}
	bool hasGoodBone = false;
	for (int i = 0; i < node->mNumChildren; i++) {
		BoneNode* child = new BoneNode(skelly,node->mChildren[i], mesh);
		if (child->kill != true) {//won't be null if has bones and stuff
			hasGoodBone = true;
			children.push_back(child);
			child->parent = this;
		}
		else delete child;
	}
	if (hasBone || hasGoodBone) {// this is a good node, populate it
		//ConvertAssimpMatrix(offset,&mesh->mBones[boneIndex]->mOffsetMatrix);
		//matrix = new glm::mat4(glm::identity<glm::mat4>());
		return;
	}
	kill = true;
}

void BoneNode::Rotate(glm::quat* quat) {
	if (index != -1) {
		glm::mat4 tmpParent = parent->index != -1 ? GETMATRIX(parent) : glm::identity<glm::mat4>();
		GETMATRIX(this) = tmpParent * glm::translate(glm::identity<glm::mat4>(),GETOFFSET(this)*glm::vec3(-1,-1,-1)) * glm::mat4_cast(*quat) * glm::translate(glm::identity<glm::mat4>(),GETOFFSET(this));

		for (BoneNode* child : children) {
			glm::quat* tmp = new glm::quat(glm::identity<glm::quat>());
			child->Rotate(tmp);
			delete tmp;
		}
	}
	else printf("This node isn't a bone! (index: %u)\n",index);
}


Skeleton::Skeleton()
{
	boneCount = 0;
	boneMatrices = nullptr;
	boneOffsets = nullptr;
	rootBone = nullptr;
}

Skeleton::Skeleton(aiNode* node, aiMesh* mesh)
{
	boneCount = mesh->mNumBones;
	if (boneCount > 32) {
		printf("WARNING - too many bones, things will be broken! (32 max, you have %d)\n", boneCount);
	}
	boneMatrices = (glm::mat4*)malloc(sizeof(glm::mat4) * boneCount);
	boneOffsets = (glm::vec3*)malloc(sizeof(glm::vec3) * boneCount);
	for (int i = 0; i < boneCount; i++) {
		const aiBone* bone = mesh->mBones[i];
		ConvertAssimpMatrix(&boneOffsets[i], (aiMatrix4x4*)&bone->mOffsetMatrix);
		boneMatrices[i] = glm::identity<glm::mat4>();
	}

	rootBone = new BoneNode(this,node, mesh);
}

void Skeleton::Rotate(unsigned int index, glm::quat* quat)
{
	BoneNode* child = Search(rootBone, index);
	if (child != nullptr) {
		child->Rotate(quat);
	}
	else printf("There is no bone of index %u in this skeleton!\n", index);
}
BoneNode* Skeleton::Search(BoneNode* node,unsigned int index) {
	if (node->index == index) { //this is the correct node
		return node;
	}
	else {
		for (BoneNode* child : node->children) { // check children for correct node
			BoneNode* rez = Search(child, index);//  check if this child is good
			if (rez != nullptr) {
				return rez;
			}
		}
		return nullptr; // if fail return nullptr
	}
}