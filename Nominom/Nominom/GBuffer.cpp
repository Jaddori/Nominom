#include "GBuffer.h"

GBuffer::GBuffer()
{
	LOG( VERBOSITY_INFORMATION, "GBuffer", "Constructing." );
}

GBuffer::~GBuffer()
{
	LOG( VERBOSITY_INFORMATION, "GBuffer", "Destructing." );
}

bool GBuffer::load( Assets* a, int w, int h )
{
	LOG( VERBOSITY_INFORMATION, "GBuffer", "Loading shaders." );

	assets = a;
	width = w;
	height = h;
	bool result = true;

	if( !geometryPass.load( "./assets/shaders/geometry_pass.vs",
							nullptr,
							"./assets/shaders/geometry_pass.fs" ) )
	{
		LOG( VERBOSITY_ERROR, "GBuffer", "Failed to load geometry pass shader." );
		result = false;
	}
	AGLOG( "GBuffer(load)" );

	if( !directionalLightPass.load( "./assets/shaders/directional_light_pass.vs",
									nullptr,
									"./assets/shaders/directional_light_pass.fs" ) )
	{
		LOG( VERBOSITY_ERROR, "GBuffer", "Failed to load directional light pass shader." );
		result = false;
	}
	AGLOG( "GBuffer(load)" );

	if( !pointLightPass.load( "./assets/shaders/point_light_pass.vs",
								nullptr,
								"./assets/shaders/point_light_pass.fs" ) )
	{
		LOG( VERBOSITY_ERROR, "GBuffer", "Failed to load point light pass shader." );
		result = false;
	}
	AGLOG( "GBuffer(load)" );

	sphereMesh = assets->loadMesh( GBUFFER_SPHERE_MESH_PATH );
	if( sphereMesh < 0 )
	{
		LOG( VERBOSITY_ERROR, "GBuffer", "Failed to load sphere mesh for point light pass." );
		result = false;
	}
	AGLOG( "GBuffer(load)" );

	/*if( !spotLightPass.load( "./assets/shaders/spot_light_pass.vs",
								"./assets/shaders/spot_light_pass.gs",
								"./assets/shaders/spot_light_pass.fs" ) )
	{
		LOG( VERBOSITY_ERROR, "GBuffer", "Failed to load spot light pass shader." );
		result = false;
	}*/

	AGLOG( "GBuffer(load)" );

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
		geometryViewMatrix = geometryPass.getUniform( "viewMatrix" );
		geometryWorldMatrices = geometryPass.getUniform( "worldMatrices" );
		GLOG( "GBuffer" );

		geometryDiffuseMap = geometryPass.getUniform( "diffuseMap" );
		geometryNormalMap = geometryPass.getUniform( "normalMap" );
		geometryPositionMap = geometryPass.getUniform( "positionMap" );
		geometryDepthMap = geometryPass.getUniform( "depthMap" );
		GLOG( "GBuffer" );

		geometryFarPlane = geometryPass.getUniform( "farPlane" );
		geometryNearPlane = geometryPass.getUniform( "nearPlane" );
		GLOG( "GBuffer" );
	}

	if( directionalLightPass.getValid() )
	{
		directionalLightPass.upload();
		GLOG( "GBuffer" );

		directionalLightDirection = directionalLightPass.getUniform( "directionalLight.direction" );
		directionalLightColor = directionalLightPass.getUniform( "directionalLight.color" );
		directionalLightIntensity = directionalLightPass.getUniform( "directionalLight.intensity" );
		directionalLightCameraPosition = directionalLightPass.getUniform( "cameraPosition" );
		directionalLightSpecularPower = directionalLightPass.getUniform( "specularPower" );
		GLOG( "GBuffer" );

		directionalLightDiffuseTarget = directionalLightPass.getUniform( "diffuseTarget" );
		directionalLightNormalTarget = directionalLightPass.getUniform( "normalTarget" );
		directionalLightPositionTarget = directionalLightPass.getUniform( "positionTarget" );
		directionalLightDepthTarget = directionalLightPass.getUniform( "depthTarget" );
		GLOG( "GBuffer" );
	}

	if( pointLightPass.getValid() )
	{
		pointLightPass.upload();
		GLOG( "GBuffer" );

		pointLightProjectionMatrix = pointLightPass.getUniform( "projectionMatrix" );
		pointLightViewMatrix = pointLightPass.getUniform( "viewMatrix" );
		pointLightWorldMatrix = pointLightPass.getUniform( "worldMatrix" );
		GLOG( "GBuffer" );

		pointLightCameraPosition = pointLightPass.getUniform( "cameraPosition" );
		pointLightSpecularPower = pointLightPass.getUniform( "specularPower" );
		pointLightScreenSize = pointLightPass.getUniform( "screenSize" );
		GLOG( "GBuffer" );

		pointLightPosition = pointLightPass.getUniform( "pointLight.position" );
		pointLightRadius = pointLightPass.getUniform( "pointLight.radius" );
		pointLightColor = pointLightPass.getUniform( "pointLight.color" );
		pointLightIntensity = pointLightPass.getUniform( "pointLight.intensity" );
		pointLightLinear = pointLightPass.getUniform( "pointLight.linear" );
		pointLightConstant = pointLightPass.getUniform( "pointLight.constant" );
		pointLightExponent = pointLightPass.getUniform( "pointLight.exponent" );
		GLOG( "GBuffer" );

		pointLightDiffuseTarget = pointLightPass.getUniform( "diffuseTarget" );
		pointLightNormalTarget = pointLightPass.getUniform( "normalTarget" );
		pointLightPositionTarget = pointLightPass.getUniform( "positionTarget" );
		GLOG( "GBuffer" );
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
	glGenTextures( 1, &depthBuffer );
	GLOG( "GBuffer" );

	// generate color targets
	for( int i=0; i<MAX_TARGETS; i++ )
	{
		glBindTexture( GL_TEXTURE_2D, targets[i] );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0 );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0 );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL );
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, targets[i], 0 );

		GLOG( "GBuffer" );
	}

	// generate depth target
	glBindTexture( GL_TEXTURE_2D, depthBuffer );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0 );
	GLOG( "GBuffer" );

	GLenum status = glCheckFramebufferStatus( GL_FRAMEBUFFER );
	if( status != GL_FRAMEBUFFER_COMPLETE )
	{
		LOG( VERBOSITY_ERROR, "GBuffer", "Failed to create framebuffer.\nStatus: %d", status );
	}

	glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	// generate quad
	glGenVertexArrays( 1, &quadVAO );
	glBindVertexArray( quadVAO );

	glEnableVertexAttribArray( 0 );
	glEnableVertexAttribArray( 1 );

	const GLfloat quadVertices[] =
	{
		// position		uv
		-1.0f, 1.0f,	0.0f, 0.0f,
		-1.0f, -1.0f,	0.0f, 1.0f,
		1.0f, 1.0f,		1.0f, 0.0f,
		1.0f, -1.0f,	1.0f, 1.0f
	};
	const int STRIDE = sizeof(GLfloat)*4;

	GLuint quadVBO;
	glGenBuffers( 1, &quadVBO );
	glBindBuffer( GL_ARRAY_BUFFER, quadVBO );
	glBufferData( GL_ARRAY_BUFFER, STRIDE*4, quadVertices, GL_STATIC_DRAW );

	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, STRIDE, 0 );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(GLfloat)*2) );

	glBindVertexArray( 0 );
}

void GBuffer::begin()
{
	glBindFramebuffer( GL_FRAMEBUFFER, fbo );
}

void GBuffer::end()
{
	if( debug )
	{
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
		glBindFramebuffer( GL_READ_FRAMEBUFFER, fbo );

		// diffuse target, top left
		glReadBuffer( GL_COLOR_ATTACHMENT0 );
		glBlitFramebuffer( 0, 0, width, height, 0, height/2, width/2, height, GL_COLOR_BUFFER_BIT, GL_LINEAR );

		// normal target, top right
		glReadBuffer( GL_COLOR_ATTACHMENT1 );
		glBlitFramebuffer( 0, 0, width, height, width/2, height/2, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR );

		// position target, bottom left
		glReadBuffer( GL_COLOR_ATTACHMENT2 );
		glBlitFramebuffer( 0, 0, width, height, 0, 0, width/2, height/2, GL_COLOR_BUFFER_BIT, GL_LINEAR );

		// depth target, bottom right
		glReadBuffer( GL_COLOR_ATTACHMENT3 );
		glBlitFramebuffer( 0, 0, width, height, width/2, 0, width, height/2, GL_COLOR_BUFFER_BIT, GL_LINEAR );
	}
	else
	{
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
		glBindFramebuffer( GL_READ_FRAMEBUFFER, fbo );
		glReadBuffer( GL_COLOR_ATTACHMENT4 );
		glBlitFramebuffer( 0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR );
		glBlitFramebuffer( 0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST );
	}

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	GLOG( "GBuffer" );
}

void GBuffer::beginGeometryPass( Camera* camera )
{
	GLenum drawBuffers[] =
	{
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3,
	};
	glDrawBuffers( MAX_TARGETS-1, drawBuffers );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	AGLOG( "GBuffer(GeometryPass)" );

	geometryPass.bind();
	geometryPass.setMat4( geometryProjectionMatrix, camera->getFinalProjectionMatrix() );
	geometryPass.setMat4( geometryViewMatrix, camera->getFinalViewMatrix() );
	AGLOG( "GBuffer(GeometryPass)" );

	geometryPass.setFloat( geometryFarPlane, camera->getFarPlane() );
	geometryPass.setFloat( geometryNearPlane, camera->getNearPlane() );
	AGLOG( "GBuffer(GeometryPass)" );

	geometryPass.setInt( geometryDiffuseMap, 0 );
	geometryPass.setInt( geometryNormalMap, 1 );
	AGLOG( "GBuffer(GeometryPass)" );
}

void GBuffer::endGeometryPass()
{
}

void GBuffer::updateGeometryWorldMatrices( const glm::mat4* worldMatrices, int count )
{
	geometryPass.setMat4( geometryWorldMatrices, worldMatrices, count );
}

void GBuffer::updateGeometryTextures( Texture* diffuseMap, Texture* normalMap, Texture* specularMap )
{
	diffuseMap->bind( GL_TEXTURE0 );
	normalMap->bind( GL_TEXTURE1 );
	specularMap->bind( GL_TEXTURE2 );
}

void GBuffer::beginDirectionalLightPass( Camera* camera )
{
	glDisable( GL_CULL_FACE );
	glDisable( GL_DEPTH_TEST );
	glDrawBuffer( GL_COLOR_ATTACHMENT4 );
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT );
	glDepthMask( GL_FALSE );

	glEnable( GL_BLEND );
	glBlendEquation( GL_FUNC_ADD );
	glBlendFunc( GL_ONE, GL_ONE );

	directionalLightPass.bind();
	AGLOG( "GBuffer(DirectionalLightPass)" );

	directionalLightPass.setVec3( directionalLightCameraPosition, camera->getPosition() );
	// TEMP: Magic number
	directionalLightPass.setFloat( directionalLightSpecularPower, 8.0f );
	AGLOG( "GBuffer(DirectionalLightPass)" );
	
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, targets[0] );
	glActiveTexture( GL_TEXTURE1 );
	glBindTexture( GL_TEXTURE_2D, targets[1] );
	glActiveTexture( GL_TEXTURE2 );
	glBindTexture( GL_TEXTURE_2D, targets[2] );
	glActiveTexture( GL_TEXTURE3 );
	glBindTexture( GL_TEXTURE_2D, targets[3] );
	AGLOG( "GBuffer(DirectionalLightPass)" );

	directionalLightPass.setInt( directionalLightDiffuseTarget, 0 );
	directionalLightPass.setInt( directionalLightNormalTarget, 1 );
	directionalLightPass.setInt( directionalLightPositionTarget, 2 );
	directionalLightPass.setInt( directionalLightDepthTarget, 3 );
	AGLOG( "GBuffer(DirectionalLightPass)" );
}

void GBuffer::endDirectionalLightPass()
{
	glDisable( GL_BLEND );
	glBlendEquation( GL_FUNC_ADD );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );
	glDepthMask( GL_TRUE );
}

//void GBuffer::renderDirectionalLight( const glm::vec3& direction, const glm::vec3& color, float intensity )
void GBuffer::renderDirectionalLight( const DirectionalLight& light )
{
	directionalLightPass.setVec3( directionalLightDirection, light.direction );
	directionalLightPass.setVec3( directionalLightColor, light.color );
	directionalLightPass.setFloat( directionalLightIntensity, light.intensity );
	AGLOG( "GBuffer(DirectionalLightPass)" );

	glBindVertexArray( quadVAO );
	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
	glBindVertexArray( 0 );
	AGLOG( "GBuffer(DirectionalLightPass)" );
}

void GBuffer::beginPointLightPass( Camera* camera )
{
	glDrawBuffer( GL_COLOR_ATTACHMENT4 );
	glDisable( GL_DEPTH_TEST );
	glCullFace( GL_FRONT );

	glDepthMask( GL_FALSE );
	glEnable( GL_BLEND );
	glBlendEquation( GL_FUNC_ADD );
	glBlendFunc( GL_ONE, GL_ONE );
	AGLOG( "GBuffer(PointLightPass)" );

	pointLightPass.bind();
	AGLOG( "GBuffer(PointLightPass)" );

	pointLightPass.setMat4( pointLightProjectionMatrix, camera->getFinalProjectionMatrix() );
	pointLightPass.setMat4( pointLightViewMatrix, camera->getFinalViewMatrix() );
	pointLightPass.setVec3( pointLightCameraPosition, camera->getPosition() );
	// TEMP: Magic numbers
	pointLightPass.setFloat( pointLightSpecularPower, 8.0f );
	pointLightPass.setVec2( pointLightScreenSize, glm::vec2( 640.0f, 480.0f ) );
	AGLOG( "GBuffer(PointLightPass)" );

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, targets[0] );
	glActiveTexture( GL_TEXTURE1 );
	glBindTexture( GL_TEXTURE_2D, targets[1] );
	glActiveTexture( GL_TEXTURE2 );
	glBindTexture( GL_TEXTURE_2D, targets[2] );
	AGLOG( "GBuffer(PointLightPass)" );

	pointLightPass.setInt( pointLightDiffuseTarget, 0 );
	pointLightPass.setInt( pointLightNormalTarget, 1 );
	pointLightPass.setInt( pointLightPositionTarget, 2 );
	AGLOG( "GBuffer(PointLightPass)" );
}

void GBuffer::endPointLightPass()
{
	glDepthMask( GL_TRUE );
	glEnable( GL_DEPTH_TEST );
	glDisable( GL_BLEND );
	glCullFace( GL_BACK );
}

//void GBuffer::renderPointLight( Camera* camera, const glm::vec3& position, float radius, const glm::vec3& color, float intensity )
void GBuffer::renderPointLight( const PointLight& light )
{
	pointLightPass.setVec3( pointLightPosition, light.position );
	pointLightPass.setVec3( pointLightColor, light.color );
	pointLightPass.setFloat( pointLightIntensity, light.intensity );
	AGLOG( "GBuffer(PointLightPass)" );

	// TEMP: Magic numbers
	pointLightPass.setFloat( pointLightLinear, 1.0f );
	pointLightPass.setFloat( pointLightConstant, 0.0f );
	pointLightPass.setFloat( pointLightExponent, 1.0f );
	AGLOG( "GBuffer(PointLightPass)" );

	// Distance from attenuation formula:
	// http://ogldev.atspace.co.uk/www/tutorial36/tutorial36.html
	float C = fmax( fmax( light.color.r, light.color.g ), light.color.b );
	float radius = ( -light.linear + sqrt( powf( light.linear, 2.0f ) - 4*light.exponent * ( light.constant - 256*C*light.intensity ) ) ) / (2*light.exponent);

	glm::mat4 worldMatrix = glm::scale( glm::translate( glm::mat4(), light.position ), glm::vec3( radius ) );
	pointLightPass.setMat4( pointLightWorldMatrix, worldMatrix );

	Mesh* sphere = assets->getMesh( sphereMesh );
	assert( sphere );

	sphere->render(1);
	AGLOG( "GBuffer(PointLightPass)" );
}

void GBuffer::setDebug( bool d )
{
	debug = d;
}

void GBuffer::toggleDebug()
{
	debug = !debug;
}

GLuint GBuffer::getFBO() const
{
	return fbo;
}