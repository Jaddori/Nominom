#pragma once

#include "BaseIncludes.h"

class ModelInstance
{
public:
	ModelInstance();
	ModelInstance( int mesh, int texture );
	~ModelInstance();

	int add();
	void remove( int index );

	void setMesh( int mesh );
	void setTexture( int texture );

	int getMesh() const;
	int getTexture() const;
	int getInstances() const;

private:
	int mesh;
	int texture;
	Array<glm::mat4> worldMatrices;
};