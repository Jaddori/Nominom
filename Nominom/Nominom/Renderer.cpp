#include "Renderer.h"

Renderer::Renderer()
	: instances( nullptr )
{
	LOG( VERBOSITY_INFORMATION, "Renderer", "Constructing." );
}

Renderer::~Renderer()
{
	LOG( VERBOSITY_INFORMATION, "Renderer", "Destructing." );
}

void Renderer::load()
{
	LOG( VERBOSITY_INFORMATION, "Renderer", "Loading shader." );

	if( !basicShader.load( "./assets/shaders/basic.vs", nullptr, "./assets/shaders/basic.fs" ) )
	{
		LOG( VERBOSITY_ERROR, "Renderer", "Failed to load basic shader." );
	}

	camera.updatePerspective( 640.0f, 480.0f );
}

void Renderer::upload()
{
	LOG( VERBOSITY_INFORMATION, "Renderer", "Uploading shader." );

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

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
	basicShader.setMat4( projectionMatrixLocation, &camera.getFinalProjectionMatrix(), 1 );
	basicShader.setMat4( viewMatrixLocation, &camera.getFinalViewMatrix(), 1 );

	const int MAX_INSTANCES = instances->getSize();
	for( int curInstance = 0; curInstance < MAX_INSTANCES; curInstance++ )
	{
		ModelInstance& instance = instances->at( curInstance );

		basicShader.setMat4( worldMatricesLocation, instance.getFinalMatrices(), instance.getInstances() );

		Texture* texture = assets->getTexture( instance.getTexture() );
		texture->bind( GL_TEXTURE0 );

		Mesh* mesh = assets->getMesh( instance.getMesh() );
		mesh->render( instance.getInstances() );
	}
}

void Renderer::finalize()
{
	const int MAX_INSTANCES = instances->getSize();
	for( int curInstance = 0; curInstance < MAX_INSTANCES; curInstance++ )
	{
		instances->at( curInstance ).finalize();
	}

	camera.finalize();
}

Camera* Renderer::getCamera()
{
	return &camera;
}