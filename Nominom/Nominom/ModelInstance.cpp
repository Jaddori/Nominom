#include "ModelInstance.h"

ModelInstance::ModelInstance()
{
}

ModelInstance::ModelInstance( int m, int t )
	: mesh( m ), texture( t )
{
}

ModelInstance::~ModelInstance()
{
}

int ModelInstance::add()
{
	int result = worldMatrices.getSize();
	worldMatrices.add( glm::mat4() );

	return result;
}

void ModelInstance::remove( int index )
{
	worldMatrices.removeAt( index );
}

void ModelInstance::setMesh( int m )
{
	mesh = m;
}

void ModelInstance::setTexture( int t )
{
	texture = t;
}

int ModelInstance::getMesh() const
{
	return mesh;
}

int ModelInstance::getTexture() const
{
	return texture;
}

int ModelInstance::getInstances() const
{
	return worldMatrices.getSize();
}