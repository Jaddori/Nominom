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

void Renderer::load( Assets* assets )
{
	LOG( VERBOSITY_INFORMATION, "Renderer", "Loading shader." );

	if( !gbuffer.load( assets, 640, 480 ) )
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

	gbuffer.upload();
}

void Renderer::queueInstances( Array<ModelInstance>* i )
{
	instances = i;
}

void Renderer::queueDirectionalLights( Array<DirectionalLight>* lights )
{
	directionalLights = lights;
}

void Renderer::queuePointLights( Array<PointLight>* lights )
{
	pointLights = lights;
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
	gbuffer.beginDirectionalLightPass( TARGET_LIGHT, &camera );

	assert( directionalLights );
	const int MAX_DIRECTIONAL_LIGHTS = directionalLights->getSize();
	for( int i=0; i<MAX_DIRECTIONAL_LIGHTS; i++ )
	{
		gbuffer.renderDirectionalLight( directionalLights->at(i) );
	}
	gbuffer.endDirectionalLightPass();

	// POINT LIGHT PASS
	gbuffer.beginPointLightPass( TARGET_LIGHT, &camera );

	assert( pointLights );
	const int NUM_POINT_LIGHTS = pointLights->getSize();
	for( int i=0; i<NUM_POINT_LIGHTS; i++ )
	{
		gbuffer.renderPointLight( pointLights->at(i) );
	}
	gbuffer.endPointLightPass();

	// BILLBOARD PASS
	gbuffer.beginBillboardPass( &camera );

	Array<Billboard> billboards;
	billboards.add( { glm::vec3( 0.0f, 3.0f, 0.0f ), glm::vec2( 1.0f, 1.0f ) } );

	Texture* diffuseMap = assets->getTexture( 3 );
	diffuseMap->bind( GL_TEXTURE0 );

	Texture* normalMap = assets->getTexture( 4 );
	normalMap->bind( GL_TEXTURE1 );

	Texture* specularMap = assets->getTexture( 5 );
	specularMap->bind( GL_TEXTURE2 );

	gbuffer.renderBillboards( &billboards );

	gbuffer.endBillboardPass();
	AGLOG( "Renderer" );

	// DIRECTIONAL LIGHT PASS
	gbuffer.beginDirectionalLightPass( TARGET_BILLBOARD, &camera );

	for( int i=0; i<MAX_DIRECTIONAL_LIGHTS; i++ )
	{
		gbuffer.renderDirectionalLight( directionalLights->at(i) );
	}
	gbuffer.endDirectionalLightPass();

	// POINT LIGHT PASS
	gbuffer.beginPointLightPass( TARGET_BILLBOARD, &camera );

	for( int i=0; i<NUM_POINT_LIGHTS; i++ )
	{
		gbuffer.renderPointLight( pointLights->at(i) );
	}
	gbuffer.endPointLightPass();

	// FINAL PASS
	gbuffer.performFinalPass();

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