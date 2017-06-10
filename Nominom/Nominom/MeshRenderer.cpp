#include "MeshRenderer.h"

MeshRenderer::MeshRenderer()
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::load( Assets* assets, Array<ModelInstance>* instances )
{
	mesh = assets->loadMesh( "./assets/meshes/test.mesh" );
	diffuseMap = assets->loadTexture( "./assets/textures/crate_diffuse.dds" );
	normalMap = assets->loadTexture( "./assets/textures/crate_normal.dds" );
	specularMap = assets->loadTexture( "./assets/textures/crate_specular.dds" );

	// TEMP: Should do this more elegantly
	instanceIndex = -1;
	for( int i=0; instances->getSize() && instanceIndex < 0; i++ )
	{
		const ModelInstance& instance = instances->at(i);
		if( instance.getMesh() == mesh &&
			instance.getDiffuseMap() == diffuseMap &&
			instance.getNormalMap() == normalMap &&
			instance.getSpecularMap() == specularMap )
		{
			instanceIndex = i;
		}
	}

	if( instanceIndex < 0 )
	{
		ModelInstance instance( mesh, diffuseMap, normalMap, specularMap );
		instanceIndex = instances->getSize();
		instances->add( instance );
	}

	worldMatrixIndex = instances->at( instanceIndex ).add();
}

void MeshRenderer::finalize( Array<ModelInstance>* instances )
{
	Transform* transform = (Transform*)parent->getComponents()->at(0);

	glm::mat4 worldMatrix = glm::scale( glm::translate( glm::mat4(), transform->getPosition() ), transform->getScale() );
	instances->at( instanceIndex ).setWorldMatrix( worldMatrixIndex, worldMatrix );
}