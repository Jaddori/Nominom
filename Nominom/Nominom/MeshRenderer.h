#pragma once

#include "Transform.h"
#include "Assets.h"
#include "ModelInstance.h"

class MeshRenderer : public Component
{
public:
	MeshRenderer();
	virtual ~MeshRenderer();

	void load( Assets* assets, Array<ModelInstance>* instances );
	void finalize( Array<ModelInstance>* instance );

private:
	int mesh;
	int diffuseMap;
	int normalMap;
	int specularMap;
	int instanceIndex;
	int worldMatrixIndex;
};