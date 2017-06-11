#pragma once

#include "Transform.h"
#include "Assets.h"
#include "InstanceHandler.h"

class MeshRenderer : public Component
{
public:
	MeshRenderer();
	virtual ~MeshRenderer();

	void load( Assets* assets, InstanceHandler* instanceHandler );
	void finalize();

private:
	int mesh;
	int diffuseMap;
	int normalMap;
	int specularMap;
	InstanceIndex instanceIndex;

	glm::mat4* worldMatrix;
	bool* dirtyFlag;
};