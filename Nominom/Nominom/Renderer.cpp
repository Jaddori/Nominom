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

	/*if( !basicShader.load( "./assets/shaders/basic.vs", nullptr, "./assets/shaders/basic.fs" ) )
	{
		LOG( VERBOSITY_ERROR, "Renderer", "Failed to load basic shader." );
	}*/

	if( !gbuffer.load( 640, 480 ) )
	{
		LOG( VERBOSITY_ERROR, "Renderer", "Failed to load gbuffer." );
	}

	camera.updatePerspective( 640.0f, 480.0f );
}

void Renderer::upload()
{
	LOG( VERBOSITY_INFORMATION, "Renderer", "Uploading shader." );

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	/*if( basicShader.getValid() )
	{
		basicShader.upload();

		worldMatricesLocation = basicShader.getUniform( "worldMatrices" );
		viewMatrixLocation = basicShader.getUniform( "viewMatrix" );
		projectionMatrixLocation = basicShader.getUniform( "projectionMatrix" );
	}*/

	gbuffer.upload();
}

void Renderer::queue( Array<ModelInstance>* i )
{
	instances = i;
}

void Renderer::render( Assets* assets )
{
	gbuffer.begin();

	// GEOMETRY PASS
	gbuffer.beginGeometryPass( &camera );

	const int MAX_INSTANCES = instances->getSize();
	for( int curInstance = 0; curInstance < MAX_INSTANCES; curInstance++ )
	{
		ModelInstance& instance = instances->at( curInstance );

		gbuffer.updateGeometryWorldMatrices( instance.getFinalMatrices(), instance.getInstances() );

		assert( instance.getDiffuseMap() >= 0 );
		assert( instance.getNormalMap() >= 0 );
		assert( instance.getSpecularMap() >= 0 );

		Texture* diffuseMap = assets->getTexture( instance.getDiffuseMap() );
		Texture* normalMap = assets->getTexture( instance.getNormalMap() );
		Texture* specularMap = assets->getTexture( instance.getSpecularMap() );

		gbuffer.updateGeometryTextures( diffuseMap, normalMap, specularMap );

		Mesh* mesh = assets->getMesh( instance.getMesh() );
		mesh->render( instance.getInstances() );
	}

	gbuffer.endGeometryPass();

	// DIRECTIONAL LIGHT PASS
	gbuffer.beginDirectionalLightPass( &camera );

	glm::vec3 direction( 1.0f, -1.0f, 1.0f );
	glm::vec3 color( 1.0f, 0.0f, 0.0f );
	float intensity = 0.8f;

	gbuffer.renderDirectionalLight( direction, color, intensity );

	direction = glm::vec3( -1.0f, -1.0f, 1.0f );
	color = glm::vec3( 0.0f, 0.0f, 1.0f );
	intensity = 0.75f;

	gbuffer.renderDirectionalLight( direction, color, intensity );

	gbuffer.endDirectionalLightPass();

	// POINT LIGHT PASS

	gbuffer.end();
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

GBuffer* Renderer::getGBuffer()
{
	return &gbuffer;
}