#include "Renderer.h"

Renderer::Renderer()
	: instances( nullptr )
{
}

Renderer::~Renderer()
{
}

void Renderer::queue( Array<ModelInstance>* i )
{
	instances = i;
}

void Renderer::render( Assets* assets )
{
	const int MAX_INSTANCES = instances->getSize();
	for( int curInstance = 0; curInstance < MAX_INSTANCES; curInstance++ )
	{
		ModelInstance& instance = instances->at( curInstance );

		Texture* texture = assets->getTexture( instance.getTexture() );
		texture->bind( GL_TEXTURE0 );

		Mesh* mesh = assets->getMesh( instance.getMesh() );
		mesh->render( instance.getInstances() );
	}
}