#include "ModelInstance.h"

ModelInstance::ModelInstance()
	: dirtyMatrices( true ), mesh( -1 ), diffuseMap( -1 ), normalMap( -1 ), specularMap( -1 )
{
}

ModelInstance::ModelInstance( int m, int diffuse, int normal, int specular )
	: mesh( m ), diffuseMap( diffuse ), normalMap( normal ), specularMap( specular ), dirtyMatrices( true )
{
}

ModelInstance::~ModelInstance()
{
}

int ModelInstance::add()
{
	int result = -1;
	const int MAX_INDICES = worldMatrices.getSize();
	for( int curIndex = 0; curIndex < MAX_INDICES && result < 0; curIndex++ )
	{
		if( flags[curIndex] == MODEL_INSTANCE_VACANT )
		{
			result = curIndex;
		}
	}

	if( result < 0 )
	{
		result = MAX_INDICES;

		worldMatrices.add( glm::mat4() );
		flags.add( MODEL_INSTANCE_VISIBLE );
	}
	else
	{
		worldMatrices[result] = glm::mat4();
		flags[result] = MODEL_INSTANCE_VISIBLE;
	}

	dirtyMatrices = true;

	return result;
}

void ModelInstance::remove( int index )
{
	flags[index] = MODEL_INSTANCE_VACANT;
	dirtyMatrices = true;
}

void ModelInstance::finalize()
{
	if( dirtyMatrices )
	{
		finalMatrices.clear();

		const int MAX_FLAGS = flags.getSize();
		for( int i = 0; i < MAX_FLAGS; i++ )
		{
			if( flags[i] == MODEL_INSTANCE_VISIBLE )
			{
				finalMatrices.add( worldMatrices[i] );
			}
		}

		dirtyMatrices = false;
	}
}

void ModelInstance::setMesh( int m )
{
	mesh = m;
}

void ModelInstance::setTextures( int diffuse, int normal, int specular )
{
	diffuseMap = diffuse;
	normalMap = normal;
	specularMap = specular;
}

void ModelInstance::setVisible( int index, int visible )
{
	assert( index >= 0 && index < flags.getSize() );
	flags[index] = visible;
	dirtyMatrices = true;
}

void ModelInstance::setWorldMatrix( int index, const glm::mat4& matrix )
{
	assert( index >= 0 && index < worldMatrices.getSize() );
	worldMatrices[index] = matrix;
}

void ModelInstance::setDirty( bool dirty )
{
	dirtyMatrices = dirty;
}

int ModelInstance::getMesh() const
{
	return mesh;
}

/*int ModelInstance::getTexture() const
{
	return texture;
}*/

int ModelInstance::getDiffuseMap() const
{
	return diffuseMap;
}

int ModelInstance::getNormalMap() const
{
	return normalMap;
}

int ModelInstance::getSpecularMap() const
{
	return specularMap;
}

int ModelInstance::getInstances() const
{
	return worldMatrices.getSize();
}

glm::mat4* ModelInstance::getWorldMatrix( int index )
{
	return worldMatrices.getData() + index;
}

const glm::mat4* ModelInstance::getWorldMatrices()
{
	return worldMatrices.getData();
}

const glm::mat4* ModelInstance::getFinalMatrices()
{
	return finalMatrices.getData();
}