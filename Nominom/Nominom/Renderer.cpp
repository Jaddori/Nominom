#include "Renderer.h"

Renderer::Renderer()
	: instances( nullptr )
{
	LOG( VERBOSITY_INFORMATION, "Renderer(constructor)", "Constructing." );
}

Renderer::~Renderer()
{
	LOG( VERBOSITY_INFORMATION, "Renderer(destructor)", "Destructing." );
}

void Renderer::load( Assets* assets )
{
	LOG( VERBOSITY_INFORMATION, "Renderer(load)", "Loading shader." );

	if( !gbuffer.load( assets, 640, 480 ) )
	{
		LOG( VERBOSITY_ERROR, "Renderer(load)", "Failed to load gbuffer." );
	}

	if( !textShader.load( "./assets/shaders/text.vs",
							"./assets/shaders/text.gs",
							"./assets/shaders/text.fs" ) )
	{
		LOG( VERBOSITY_ERROR, "Renderer(load)", "Failed to load text shader." );
	}

	// TEMP: Magic numbers
	perspectiveCamera.updatePerspective( 640.0f, 480.0f );
	orthographicCamera.updateOrthographic( 640.0f, 480.0f );
}

void Renderer::upload()
{
	LOG( VERBOSITY_INFORMATION, "Renderer(upload)", "Uploading shader." );

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	gbuffer.upload();

	if( textShader.getValid() )
	{
		textShader.upload();
		AGLOG( "Renderer(upload)" );

		textProjectionMatrix = textShader.getUniform( "projectionMatrix" );
		textDiffuseMap = textShader.getUniform( "diffuseMap" );
	}
}

void Renderer::queueInstances( Array<ModelInstance>* i )
{
	instances = i;
}

void Renderer::queueTextInstances( Array<TextInstance>* t )
{
	textInstances = t;
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
	gbuffer.beginGeometryPass( &perspectiveCamera );

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
	assert( directionalLights );
	const int NUM_DIRECTIONAL_LIGHTS = directionalLights->getSize();
	for( int i=0; i<NUM_DIRECTIONAL_LIGHTS; i++ )
	{
		const DirectionalLight& light = directionalLights->at(i);

		// RENDER SHADOW
		gbuffer.beginDirectionalShadowPass( &perspectiveCamera, light );
		
		for( int curInstance = 0; curInstance < MAX_INSTANCES; curInstance++ )
		{
			ModelInstance& instance = instances->at( curInstance );

			gbuffer.updateDirectionalShadowWorldMatrices( instance.getFinalMatrices(), instance.getInstances() );

			Mesh* mesh = assets->getMesh( instance.getMesh() );
			mesh->render( instance.getInstances() );
		}

		gbuffer.endDirectionalShadowPass();

		// RENDER LIGHT
		gbuffer.beginDirectionalLightPass( TARGET_LIGHT, &perspectiveCamera );
		gbuffer.renderDirectionalLight( &perspectiveCamera, light );
		gbuffer.endDirectionalLightPass();
	}

	// POINT LIGHT PASS
#if 1
	gbuffer.beginPointLightPass( TARGET_LIGHT, &perspectiveCamera );

	assert( pointLights );
	const int NUM_POINT_LIGHTS = pointLights->getSize();
	for( int i=0; i<NUM_POINT_LIGHTS; i++ )
	{
		gbuffer.renderPointLight( pointLights->at(i) );
	}

	gbuffer.endPointLightPass();
#endif

	// DEBUG SHAPES
	glDrawBuffer( GL_COLOR_ATTACHMENT0+TARGET_LIGHT );

	glDisable( GL_DEPTH_TEST );
	debugShapes.bindDepthTarget( gbuffer.getTarget( TARGET_DEPTH ) );
	debugShapes.render( &perspectiveCamera );
	glEnable( GL_DEPTH_TEST );

	// BILLBOARD PASS
#if 1
	gbuffer.beginBillboardPass( &perspectiveCamera );

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
	AGLOG( "Renderer(render)" );
#endif

	// FINAL PASS
	gbuffer.performFinalPass();

	gbuffer.end();
	AGLOG( "Renderer(render)" );

	// TEXT RENDERING
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	textShader.bind();
	textShader.setMat4( textProjectionMatrix, orthographicCamera.getFinalProjectionMatrix() );

	// TEMP: Crazy magic numbers
	Texture* textTexture = assets->getTexture( 6 );
	textTexture->bind( GL_TEXTURE0 );
	textShader.setInt( textDiffuseMap, 0 );
	
	const int NUM_TEXT_INSTANCES = textInstances->getSize();
	for( int i=0; i<NUM_TEXT_INSTANCES; i++ )
	{
		textInstances->at( i ).render();
	}
	glDisable( GL_BLEND );
	AGLOG( "Renderer(render)" );
}

void Renderer::finalize()
{
	const int NUM_INSTANCES = instances->getSize();
	for( int curInstance = 0; curInstance < NUM_INSTANCES; curInstance++ )
	{
		instances->at( curInstance ).finalize();
	}

	const int NUM_TEXT_INSTANCES = textInstances->getSize();
	for( int curInstance = 0; curInstance < NUM_TEXT_INSTANCES; curInstance++ )
	{
		textInstances->at( curInstance ).finalize();
	}

	perspectiveCamera.finalize();
	orthographicCamera.finalize();

	debugShapes.finalize();
}

Camera* Renderer::getPerspectiveCamera()
{
	return &perspectiveCamera;
}

Camera* Renderer::getOrthographicCamera()
{
	return &orthographicCamera;
}

GBuffer* Renderer::getGBuffer()
{
	return &gbuffer;
}

DebugShapes* Renderer::getDebugShapes()
{
	return &debugShapes;
}