#include "GBuffer.h"

GBuffer::GBuffer()
{
	LOG( VERBOSITY_INFORMATION, "GBuffer", "Constructing." );
}

GBuffer::~GBuffer()
{
	LOG( VERBOSITY_INFORMATION, "GBuffer", "Destructing." );
}

bool GBuffer::load( int w, int h )
{
	LOG( VERBOSITY_INFORMATION, "GBuffer", "Loading shaders." );

	GLenum glError = glGetError();
	if( glError )
	{
		LOG( VERBOSITY_WARNING, "GBuffer", "Unhandled OpenGL error: %d", glError );
	}

	width = w;
	height = h;
	bool result = true;

	if( !geometryPass.load( "./assets/shaders/geometry_pass.vs",
							"./assets/shaders/geometry_pass.gs",
							"./assets/shaders/geometry_pass.fs" ) )
	{
		LOG( VERBOSITY_ERROR, "GBuffer", "Failed to load geometry pass shader." );
		result = false;
	}

	/*if( !directionalLightPass.load( "./assets/shaders/directional_light_pass.vs",
									"./assets/shaders/directional_light_pass.gs",
									"./assets/shaders/directional_light_pass.fs" ) )
	{
		LOG( VERBOSITY_ERROR, "GBuffer", "Failed to load directional light pass shader." );
		result = false;
	}

	if( !pointLightPass.load( "./assets/shaders/point_light_pass.vs",
								"./assets/shaders/point_light_pass.gs",
								"./assets/shaders/point_light_pass.fs" ) )
	{
		LOG( VERBOSITY_ERROR, "GBuffer", "Failed to load point light pass shader." );
		result = false;
	}

	if( !spotLightPass.load( "./assets/shaders/spot_light_pass.vs",
								"./assets/shaders/spot_light_pass.gs",
								"./assets/shaders/spot_light_pass.fs" ) )
	{
		LOG( VERBOSITY_ERROR, "GBuffer", "Failed to load spot light pass shader." );
		result = false;
	}*/

	return result;
}

void GBuffer::upload()
{
	LOG( VERBOSITY_INFORMATION, "GBuffer", "Uploading shaders." );

	if( geometryPass.getValid() )
	{
		geometryPass.upload();
		GLOG( "GBuffer" );

		geometryProjectionMatrix = geometryPass.getUniform( "projectionMatrix" );
		GLOG( "GBuffer" );

		geometryViewMatrix = geometryPass.getUniform( "viewMatrix" );
		GLOG( "GBuffer" );

		geometryWorldMatrices = geometryPass.getUniform( "worldMatrices" );
		GLOG( "GBuffer" );
	}

	if( directionalLightPass.getValid() )
	{
		directionalLightPass.upload();
	}

	if( pointLightPass.getValid() )
	{
		pointLightPass.upload();
	}

	if( spotLightPass.getValid() )
	{
		spotLightPass.upload();
	}

	LOG( VERBOSITY_INFORMATION, "GBuffer", "Generating FBO." );

	glGenFramebuffers( 1, &fbo );
	glBindFramebuffer( GL_FRAMEBUFFER, fbo );
	GLOG( "GBuffer" );

	glGenTextures( MAX_TARGETS, targets );
	GLOG( "GBuffer" );

	// generate color targets
	for( int i=0; i<MAX_TARGETS-1; i++ )
	{
		glBindTexture( GL_TEXTURE_2D, targets[i] );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL );
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, targets[i], 0 );

		GLOG( "GBuffer" );
	}

	// generate depth target
	glBindTexture( GL_TEXTURE_2D, targets[TARGET_DEPTH] );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, targets[TARGET_DEPTH], 0 );
	GLOG( "GBuffer" );

	GLenum drawBuffers[] =
	{
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
	};
	glDrawBuffers( MAX_TARGETS-1, drawBuffers );
	GLOG( "GBuffer" );

	GLenum status = glCheckFramebufferStatus( GL_FRAMEBUFFER );
	if( status != GL_FRAMEBUFFER_COMPLETE )
	{
		LOG( VERBOSITY_ERROR, "GBuffer", "Failed to create framebuffer.\nStatus: %d", status );
	}

	glClearColor( 0.0f, 0.0f, 1.0f, 1.0f );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

void GBuffer::begin()
{
	glBindFramebuffer( GL_FRAMEBUFFER, fbo );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void GBuffer::end()
{
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
	glBindFramebuffer( GL_READ_FRAMEBUFFER, fbo );
	glReadBuffer( GL_COLOR_ATTACHMENT0 );
	glBlitFramebuffer( 0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST );

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	GLOG( "GBuffer" );
}

void GBuffer::beginGeometryPass( Camera* camera )
{
	geometryPass.bind();
	geometryPass.setMat4( geometryProjectionMatrix, &camera->getFinalProjectionMatrix(), 1 );
	geometryPass.setMat4( geometryViewMatrix, &camera->getFinalViewMatrix(), 1 );
	geometryPass.setVec3( geometryCameraPosition, camera->getPosition() );
}

void GBuffer::endGeometryPass()
{

}

void GBuffer::updateGeometryWorldMatrices( const glm::mat4* worldMatrices, int count )
{
	geometryPass.setMat4( geometryWorldMatrices, worldMatrices, count );
}

/*void GBuffer::renderGeometry( Camera* camera, Array<ModelInstance>& instances )
{

}*/

void GBuffer::renderDirectionalLights( Camera* camera )
{
}

void GBuffer::renderPointLights( Camera* camera )
{
}

void GBuffer::renderSpotLights( Camera* camera )
{
}

GLuint GBuffer::getFBO() const
{
	return fbo;
}