#pragma once

#include "ModelInstance.h"

struct InstanceIndex
{
	int instance;
	int worldMatrix;
};

class InstanceHandler
{
public:
	InstanceHandler();
	~InstanceHandler();

	InstanceIndex add( int mesh, int diffuseMap, int normalMap, int specularMap );

	ModelInstance* getInstance( int index );

	Array<ModelInstance>* getInstances();

private:
	Array<ModelInstance> instances;
};