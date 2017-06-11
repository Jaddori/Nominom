#include "MeshRenderer.h"

MeshRenderer::MeshRenderer()
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::load( Assets* assets, InstanceHandler* instanceHandler )
{
	mesh = assets->loadMesh( "./assets/meshes/test.mesh" );
	diffuseMap = assets->loadTexture( "./assets/textures/crate_diffuse.dds" );
	normalMap = assets->loadTexture( "./assets/textures/crate_normal.dds" );
	specularMap = assets->loadTexture( "./assets/textures/crate_specular.dds" );

	instanceIndex = instanceHandler->add( mesh, diffuseMap, normalMap, specularMap );

	ModelInstance* instance = instanceHandler->getInstance( instanceIndex.instance );
	worldMatrix = instance->getWorldMatrix( instanceIndex.worldMatrix );
	dirtyFlag = instance->getDirtyFlag();
}

void MeshRenderer::finalize( InstanceHandler* instanceHandler )
{
	Transform* transform = (Transform*)parent->getComponents()->at(0);
	if( transform->getDirtyFlag() )
	{
		*worldMatrix = glm::scale( glm::translate( glm::mat4(), transform->getPosition() ), transform->getScale() );
		*dirtyFlag = true;

		transform->setDirtyFlag( false );
	}
}