#include "Renderer.h"

Renderer::Renderer()
	: instances( nullptr )
{
}

Renderer::~Renderer()
{
}

void Renderer::load()
{
	if( !basicShader.load( "./assets/shaders/basic.vs", nullptr, "./assets/shaders/basic.fs" ) )
	{
		printf( "Renderer: Failed to load basic shader.\n" );
	}

	camera.updatePerspective( 640.0f, 480.0f );
}

void Renderer::upload()
{
	if( basicShader.getValid() )
	{
		basicShader.upload();

		worldMatricesLocation = basicShader.getUniform( "worldMatrices" );
		viewMatrixLocation = basicShader.getUniform( "viewMatrix" );
		projectionMatrixLocation = basicShader.getUniform( "projectionMatrix" );
	}
}

void Renderer::queue( Array<ModelInstance>* i )
{
	instances = i;
}

void Renderer::render( Assets* assets )
{
	basicShader.bind();
	basicShader.setMat4( projectionMatrixLocation, &camera.getProjectionMatrix(), 1 );
	basicShader.setMat4( viewMatrixLocation, &camera.getViewMatrix(), 1 );

	const int MAX_INSTANCES = instances->getSize();
	for( int curInstance = 0; curInstance < MAX_INSTANCES; curInstance++ )
	{
		ModelInstance& instance = instances->at( curInstance );

		// TEMP: Move this somewhere else:
		instance.finalize();

		basicShader.setMat4( worldMatricesLocation, instance.getFinalMatrices(), instance.getInstances() );

		Texture* texture = assets->getTexture( instance.getTexture() );
		texture->bind( GL_TEXTURE0 );

		Mesh* mesh = assets->getMesh( instance.getMesh() );
		mesh->render( instance.getInstances() );
	}
}

Camera* Renderer::getCamera()
{
	return &camera;
}