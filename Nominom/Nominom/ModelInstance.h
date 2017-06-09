#pragma once

#include "BaseIncludes.h"

enum
{
	MODEL_INSTANCE_NONE = 0,
	MODEL_INSTANCE_VISIBLE,
	MODEL_INSTANCE_VACANT,
};

class ModelInstance
{
public:
	ModelInstance();
	ModelInstance( int mesh, int diffuseMap, int normalMap, int specularMap );
	~ModelInstance();

	int add();
	void remove( int index );
	void finalize();

	void setMesh( int mesh );
	void setTextures( int diffuse, int normal, int specular );
	void setVisible( int index, int visible );
	void setWorldMatrix( int index, const glm::mat4& matrix );
	void setDirty( bool dirty );

	int getMesh() const;
	//int getTexture() const;
	int getDiffuseMap() const;
	int getNormalMap() const;
	int getSpecularMap() const;
	int getInstances() const;
	glm::mat4* getWorldMatrix( int index );

	const glm::mat4* getWorldMatrices();
	const glm::mat4* getFinalMatrices();

private:
	int mesh;
	int diffuseMap;
	int normalMap;
	int specularMap;
	Array<glm::mat4> worldMatrices;
	Array<int> flags;

	Array<glm::mat4> finalMatrices;
	bool dirtyMatrices;
};