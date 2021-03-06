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
		LOG( VERBOSITY_ERROR, "GBuffer(load)", "Failed to load geometry pass shader." );
		result = false;
	}
	AGLOG( "GBuffer(load)" );

	if( !directionalLightPass.load( "./assets/shaders/directional_light_pass.vs",
									nullptr,
									"./assets/shaders/directional_light_pass.fs" ) )
	{
		LOG( VERBOSITY_ERROR, "GBuffer(load)", "Failed to load directional light pass shader." );
		result = false;
	}
	AGLOG( "GBuffer(load)" );

	if( !directionalShadowPass.load( "./assets/shaders/directional_shadow_pass.vs",
										nullptr,
									"./assets/shaders/directional_shadow_pass.fs" ) )
	{
		LOG( VERBOSITY_ERROR, "GBuffer(load)", "Failed to load directional shadow pass shader." );
		result = false;
	}

	if( !pointLightPass.load( "./assets/shaders/point_light_pass.vs",
								nullptr,
								"./assets/shaders/point_light_pass.fs" ) )
	{
		LOG( VERBOSITY_ERROR, "GBuffer(load)", "Failed to load point light pass shader." );
		result = false;
	}
	AGLOG( "GBuffer(load)" );

	sphereMesh = assets->loadMesh( GBUFFER_SPHERE_MESH_PATH );
	if( sphereMesh < 0 )
	{
		LOG( VERBOSITY_ERROR, "GBuffer(load)", "Failed to load sphere mesh for point light pass." );
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

	if( !billboardPass.load( "./assets/shaders/billboard_pass.vs",
								"./assets/shaders/billboard_pass.gs",
								"./assets/shaders/billboard_pass.fs" ) )
	{
		LOG( VERBOSITY_ERROR, "GBuffer(load)", "Failed to load billboard pass shader." );
		result = false;
	}
	AGLOG( "GBuffer(load)" );

	if( !finalPass.load( "./assets/shaders/final_pass.vs",
							nullptr,
							"./assets/shaders/final_pass.fs" ) )
	{
		LOG( VERBOSITY_ERROR, "GBuffer(load)", "Failed to load final pass shader." );
		result = false;
	}
	AGLOG( "GBuffer(load)" );

	return result;
}

void GBuffer::upload()
{
	LOG( VERBOSITY_INFORMATION, "GBuffer", "Uploading shaders." );

	if( geometryPass.getValid() )
	{
		geometryPass.upload();
		AGLOG( "GBuffer(upload)" );

		geometryProjectionMatrix = geometryPass.getUniform( "projectionMatrix" );
		geometryViewMatrix = geometryPass.getUniform( "viewMatrix" );
		geometryWorldMatrices = geometryPass.getUniform( "worldMatrices" );
		AGLOG( "GBuffer(upload)" );

		geometryDiffuseMap = geometryPass.getUniform( "diffuseMap" );
		geometryNormalMap = geometryPass.getUniform( "normalMap" );
		geometryPositionMap = geometryPass.getUniform( "positionMap" );
		geometryDepthMap = geometryPass.getUniform( "depthMap" );
		AGLOG( "GBuffer(upload)" );
	}

	if( directionalLightPass.getValid() )
	{
		directionalLightPass.upload();
		AGLOG( "GBuffer(upload)" );

		directionalLightDirection = directionalLightPass.getUniform( "directionalLight.direction" );
		directionalLightColor = directionalLightPass.getUniform( "directionalLight.color" );
		directionalLightIntensity = directionalLightPass.getUniform( "directionalLight.intensity" );
		directionalLightCameraPosition = directionalLightPass.getUniform( "cameraPosition" );
		directionalLightSpecularPower = directionalLightPass.getUniform( "specularPower" );
		directionalLightTransformation = directionalLightPass.getUniform( "lightTransformation" );
		AGLOG( "GBuffer(upload)" );

		directionalLightDiffuseTarget = directionalLightPass.getUniform( "diffuseTarget" );
		directionalLightPositionTarget = directionalLightPass.getUniform( "positionTarget" );
		directionalLightNormalTarget = directionalLightPass.getUniform( "normalTarget" );
		directionalLightDepthTarget = directionalLightPass.getUniform( "depthTarget" );
		directionalLightShadowTarget = directionalLightPass.getUniform( "shadowTarget" );
		AGLOG( "GBuffer(upload)" );
	}

	if( directionalShadowPass.getValid() )
	{
		directionalShadowPass.upload();
		AGLOG( "GBuffer(upload)" );

		directionalShadowProjectionMatrix = directionalShadowPass.getUniform( "projectionMatrix" );
		directionalShadowViewMatrix = directionalShadowPass.getUniform( "viewMatrix" );
		directionalShadowWorldMatrices = directionalShadowPass.getUniform( "worldMatrices" );
		AGLOG( "GBuffer(upload)" );
	}

	if( pointLightPass.getValid() )
	{
		pointLightPass.upload();
		AGLOG( "GBuffer(upload)" );

		pointLightProjectionMatrix = pointLightPass.getUniform( "projectionMatrix" );
		pointLightViewMatrix = pointLightPass.getUniform( "viewMatrix" );
		pointLightWorldMatrix = pointLightPass.getUniform( "worldMatrix" );
		AGLOG( "GBuffer(upload)" );

		pointLightCameraPosition = pointLightPass.getUniform( "cameraPosition" );
		pointLightScreenSize = pointLightPass.getUniform( "screenSize" );
		pointLightSpecularPower = pointLightPass.getUniform( "specularPower" );
		AGLOG( "GBuffer(upload)" );

		pointLightPosition = pointLightPass.getUniform( "pointLight.position" );
		pointLightRadius = pointLightPass.getUniform( "pointLight.radius" );
		pointLightColor = pointLightPass.getUniform( "pointLight.color" );
		pointLightIntensity = pointLightPass.getUniform( "pointLight.intensity" );
		pointLightLinear = pointLightPass.getUniform( "pointLight.linear" );
		pointLightConstant = pointLightPass.getUniform( "pointLight.constant" );
		pointLightExponent = pointLightPass.getUniform( "pointLight.exponent" );
		AGLOG( "GBuffer(upload)" );

		pointLightDiffuseTarget = pointLightPass.getUniform( "diffuseTarget" );
		pointLightPositionTarget = pointLightPass.getUniform( "positionTarget" );
		pointLightNormalTarget = pointLightPass.getUniform( "normalTarget" );
		AGLOG( "GBuffer(upload)" );
	}

	if( spotLightPass.getValid() )
	{
		spotLightPass.upload();
	}

	if( billboardPass.getValid() )
	{
		billboardPass.upload();
		AGLOG( "GBuffer(load, billboard pass)" );

		billboardProjectionMatrix = billboardPass.getUniform( "projectionMatrix" );
		billboardViewMatrix = billboardPass.getUniform( "viewMatrix" );
		billboardScreenSize = billboardPass.getUniform( "screenSize" );
		AGLOG( "GBuffer(load, billboard pass)" );

		billboardDiffuseMap = billboardPass.getUniform( "diffuseMap" );
		billboardNormalMap = billboardPass.getUniform( "normalMap" );
		billboardSpecularMap = billboardPass.getUniform( "specularMap" );
		billboardDepthTarget = billboardPass.getUniform( "depthTarget" );
		AGLOG( "GBuffer(load, billboard pass)" );

		glGenVertexArrays( 1, &billboardVAO );
		glBindVertexArray( billboardVAO );

		glEnableVertexAttribArray( 0 );
		glEnableVertexAttribArray( 1 );

		AGLOG( "GBuffer(load, billboard pass)" );

		glGenBuffers( 1, &billboardVBO );
		glBindBuffer( GL_ARRAY_BUFFER, billboardVBO );

		const int BILLBOARD_STRIDE = sizeof(Billboard);
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, BILLBOARD_STRIDE, 0 );
		glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, BILLBOARD_STRIDE, (void*)(sizeof(GLfloat)*3) );

		glBindVertexArray( 0 );
		AGLOG( "GBuffer(load, billboard pass)" );
	}

	if( finalPass.getValid() )
	{
		finalPass.upload();
		AGLOG( "GBuffer(load, final pass)" );

		finalLightTarget = finalPass.getUniform( "lightTarget" );
		finalBillboardTarget = finalPass.getUniform( "billboardTarget" );
		finalBillboardAlphaTarget = finalPass.getUniform( "billboardAlphaTarget" );
		AGLOG( "GBuffer(load, final pass)" );
	}

	LOG( VERBOSITY_INFORMATION, "GBuffer", "Generating FBO." );

	glGenFramebuffers( 1, &fbo );
	glBindFramebuffer( GL_FRAMEBUFFER, fbo );
	AGLOG( "GBuffer(upload)" );

	glGenTextures( MAX_TARGETS, targets );
	glGenTextures( 1, &depthBuffer );
	AGLOG( "GBuffer(upload)" );

	// generate color targets
	for( int i=0; i<MAX_TARGETS; i++ )
	{
		glBindTexture( GL_TEXTURE_2D, targets[i] );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0 );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0 );

		if( i == TARGET_DEPTH )
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		}

		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL );
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, targets[i], 0 );

		AGLOG( "GBuffer(upload)" );
	}

	// generate depth target
	glBindTexture( GL_TEXTURE_2D, depthBuffer );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0 );
	AGLOG( "GBuffer(upload)" );

	GLenum status = glCheckFramebufferStatus( GL_FRAMEBUFFER );
	if( status != GL_FRAMEBUFFER_COMPLETE )
	{
		LOG( VERBOSITY_ERROR, "GBuffer(upload)", "Failed to create framebuffer.\nStatus: %d", status );
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
	AGLOG( "GBuffer(upload)" );
}

void GBuffer::begin()
{
	glBindFramebuffer( GL_FRAMEBUFFER, fbo );

	// clear geometry targets
	GLenum geometryTargets[] =
	{
		GL_COLOR_ATTACHMENT0+TARGET_DIFFUSE,
		GL_COLOR_ATTACHMENT0+TARGET_POSITION,
		GL_COLOR_ATTACHMENT0+TARGET_NORMAL,
	};
	glDrawBuffers( 3, geometryTargets );
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// clear final target
	GLenum finalTargets[] =
	{
		GL_COLOR_ATTACHMENT0+TARGET_LIGHT,
		GL_COLOR_ATTACHMENT0+TARGET_BILLBOARD,
		GL_COLOR_ATTACHMENT0+TARGET_FINAL
	};
	glDrawBuffers( 3, finalTargets );
	glClear( GL_COLOR_BUFFER_BIT );

	// clear depth target to white
	glDrawBuffer( GL_COLOR_ATTACHMENT0+TARGET_DEPTH );
	glClearColor( 1.0f, 1.0f, 1.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT );

	// clear shadow target
	/*glDrawBuffer( GL_COLOR_ATTACHMENT0+TARGET_SHADOW );
	glClear( GL_COLOR_BUFFER_BIT );*/
}

void GBuffer::end()
{
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
	glBindFramebuffer( GL_READ_FRAMEBUFFER, fbo );

	switch( debugMode )
	{
		case DEBUG_GEOMETRY:
		{
			// diffuse target, top left
			glReadBuffer( GL_COLOR_ATTACHMENT0+TARGET_DIFFUSE );
			glBlitFramebuffer( 0, 0, width, height, 0, height/2, width/2, height, GL_COLOR_BUFFER_BIT, GL_LINEAR );

			// normal target, top right
			glReadBuffer( GL_COLOR_ATTACHMENT0+TARGET_NORMAL );
			glBlitFramebuffer( 0, 0, width, height, width/2, height/2, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR );

			// position target, bottom left
			glReadBuffer( GL_COLOR_ATTACHMENT0+TARGET_POSITION );
			glBlitFramebuffer( 0, 0, width, height, 0, 0, width/2, height/2, GL_COLOR_BUFFER_BIT, GL_LINEAR );

			// depth target, bottom right
			glReadBuffer( GL_COLOR_ATTACHMENT0+TARGET_DEPTH );
			glBlitFramebuffer( 0, 0, width, height, width/2, 0, width, height/2, GL_COLOR_BUFFER_BIT, GL_LINEAR );
			AGLOG( "GBuffer(end)" );
		} break;

		case DEBUG_FINAL:
		{
			// final light
			glReadBuffer( GL_COLOR_ATTACHMENT0+TARGET_LIGHT );
			glBlitFramebuffer( 0, 0, width, height, 0, height/2, width/2, height, GL_COLOR_BUFFER_BIT, GL_LINEAR );

			// final billboard
			glReadBuffer( GL_COLOR_ATTACHMENT0+TARGET_BILLBOARD );
			glBlitFramebuffer( 0, 0, width, height, width/2, height/2, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR );

			// final
			glReadBuffer( GL_COLOR_ATTACHMENT0+TARGET_FINAL );
			glBlitFramebuffer( 0, 0, width, height, 0, 0, width/2, height/2, GL_COLOR_BUFFER_BIT, GL_LINEAR );

			// alpha
			glReadBuffer( GL_COLOR_ATTACHMENT0+TARGET_ALPHA );
			glBlitFramebuffer( 0, 0, width, height, width/2, 0, width, height/2, GL_COLOR_BUFFER_BIT, GL_LINEAR );
			AGLOG( "GBuffer(end)" );
		} break;

		case DEBUG_NONE:
		default:
		{
			glReadBuffer( GL_COLOR_ATTACHMENT0+TARGET_FINAL );
			glBlitFramebuffer( 0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR );
			glBlitFramebuffer( 0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST );
			AGLOG( "GBuffer(end)" );
		} break;
	}

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	GLOG( "GBuffer(end)" );
}

void GBuffer::beginGeometryPass( Camera* camera )
{
	GLenum drawBuffers[] =
	{
		GL_COLOR_ATTACHMENT0+TARGET_DIFFUSE,
		GL_COLOR_ATTACHMENT0+TARGET_POSITION,
		GL_COLOR_ATTACHMENT0+TARGET_NORMAL,
		GL_COLOR_ATTACHMENT0+TARGET_DEPTH,
	};
	glDrawBuffers( TARGET_DEPTH+1, drawBuffers );
	//glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	AGLOG( "GBuffer(beginGeometryPass)" );

	geometryPass.bind();
	geometryPass.setMat4( geometryProjectionMatrix, camera->getFinalProjectionMatrix() );
	geometryPass.setMat4( geometryViewMatrix, camera->getFinalViewMatrix() );
	AGLOG( "GBuffer(beginGeometryPass)" );

	geometryPass.setInt( geometryDiffuseMap, 0 );
	geometryPass.setInt( geometryNormalMap, 1 );
	AGLOG( "GBuffer(beginGeometryPass)" );
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

void GBuffer::beginDirectionalLightPass( int target, Camera* camera )
{
	glDisable( GL_CULL_FACE );
	glDisable( GL_DEPTH_TEST );
	glDrawBuffer( GL_COLOR_ATTACHMENT0+target );
	/*glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT );*/
	glDepthMask( GL_FALSE );

	glEnable( GL_BLEND );
	glBlendEquation( GL_FUNC_ADD );
	glBlendFunc( GL_ONE, GL_ONE );

	directionalLightPass.bind();
	AGLOG( "GBuffer(beginDirectionalLightPass)" );

	directionalLightPass.setVec3( directionalLightCameraPosition, camera->getPosition() );
	// TEMP: Magic number
	directionalLightPass.setFloat( directionalLightSpecularPower, 8.0f );
	AGLOG( "GBuffer(beginDirectionalLightPass)" );
	
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, targets[TARGET_DIFFUSE] );
	glActiveTexture( GL_TEXTURE1 );
	glBindTexture( GL_TEXTURE_2D, targets[TARGET_POSITION] );
	glActiveTexture( GL_TEXTURE2 );
	glBindTexture( GL_TEXTURE_2D, targets[TARGET_NORMAL] );
	glActiveTexture( GL_TEXTURE3 );
	glBindTexture( GL_TEXTURE_2D, targets[TARGET_DEPTH] );
	glActiveTexture( GL_TEXTURE4 );
	glBindTexture( GL_TEXTURE_2D, targets[TARGET_SHADOW] );
	AGLOG( "GBuffer(beginDirectionalLightPass)" );

	directionalLightPass.setInt( directionalLightDiffuseTarget, 0 );
	directionalLightPass.setInt( directionalLightPositionTarget, 1 );
	directionalLightPass.setInt( directionalLightNormalTarget, 2 );
	directionalLightPass.setInt( directionalLightDepthTarget, 3 );
	directionalLightPass.setInt( directionalLightShadowTarget, 4 );
	AGLOG( "GBuffer(beginDirectionalLightPass)" );
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

void GBuffer::renderDirectionalLight( Camera* camera, const DirectionalLight& light )
{
	directionalLightPass.setVec3( directionalLightDirection, light.direction );
	directionalLightPass.setVec3( directionalLightColor, light.color );
	directionalLightPass.setFloat( directionalLightIntensity, light.intensity );

	// TEMP: Lots of magic numbers
	float halfResolution = GBUFFER_SHADOW_MAP_RESOLUTION * 0.5f;
	glm::mat4 projectionMatrix = glm::ortho( -halfResolution, halfResolution, -halfResolution, halfResolution, 0.01f, 100.0f );
	glm::mat4 viewMatrix = glm::lookAt( -light.direction*10.0f, glm::vec3( 0.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
	glm::mat4 lightTransform = projectionMatrix * viewMatrix;
	directionalLightPass.setMat4( directionalLightTransformation, lightTransform );

	AGLOG( "GBuffer(renderDirectionalLight)" );

	glBindVertexArray( quadVAO );
	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
	glBindVertexArray( 0 );
	AGLOG( "GBuffer(renderDirectionalLight)" );
}

void GBuffer::beginDirectionalShadowPass( Camera* camera, const DirectionalLight& light )
{
	glDrawBuffer( GL_COLOR_ATTACHMENT0 + TARGET_SHADOW );
	glClearColor( 1.0f, 1.0f, 1.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	AGLOG( "GBuffer(beginDirectionalShadow)" );

	directionalShadowPass.bind();
	AGLOG( "GBuffer(beginDirectionalShadow)" );

	// TEMP: Lots of magic numbers
	float halfResolution = GBUFFER_SHADOW_MAP_RESOLUTION * 0.5f;
	glm::mat4 projectionMatrix = glm::ortho( -halfResolution, halfResolution, -halfResolution, halfResolution, 0.01f, 100.0f );
	glm::mat4 viewMatrix = glm::lookAt( -light.direction*10.0f, glm::vec3( 0.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ) );

	directionalShadowPass.setMat4( directionalShadowProjectionMatrix, projectionMatrix );
	directionalShadowPass.setMat4( directionalShadowViewMatrix, viewMatrix );
	AGLOG( "GBuffer(beginDirectionalShadow)" );
}

void GBuffer::endDirectionalShadowPass()
{
}

void GBuffer::updateDirectionalShadowWorldMatrices( const glm::mat4* worldMatrices, int count )
{
	directionalShadowPass.setMat4( directionalShadowWorldMatrices, worldMatrices, count );
}

void GBuffer::clearShadowTarget()
{
	glDrawBuffer( GL_COLOR_ATTACHMENT0+TARGET_SHADOW );
	glClearColor( 1.0f, 1.0f, 1.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void GBuffer::beginPointLightPass( int target, Camera* camera )
{
	glDrawBuffer( GL_COLOR_ATTACHMENT0+target );
	glDisable( GL_DEPTH_TEST );
	glCullFace( GL_FRONT );

	glDepthMask( GL_FALSE );
	glEnable( GL_BLEND );
	glBlendEquation( GL_FUNC_ADD );
	glBlendFunc( GL_ONE, GL_ONE );
	AGLOG( "GBuffer(beginPointLightPass)" );

	pointLightPass.bind();
	AGLOG( "GBuffer(beginPointLightPass)" );

	pointLightPass.setMat4( pointLightProjectionMatrix, camera->getFinalProjectionMatrix() );
	pointLightPass.setMat4( pointLightViewMatrix, camera->getFinalViewMatrix() );
	pointLightPass.setVec3( pointLightCameraPosition, camera->getPosition() );
	// TEMP: Magic numbers
	pointLightPass.setVec2( pointLightScreenSize, glm::vec2( 640.0f, 480.0f ) );
	pointLightPass.setFloat( pointLightSpecularPower, 8.0f );
	AGLOG( "GBuffer(beginPointLightPass)" );

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, targets[TARGET_DIFFUSE] );
	glActiveTexture( GL_TEXTURE1 );
	glBindTexture( GL_TEXTURE_2D, targets[TARGET_POSITION] );
	glActiveTexture( GL_TEXTURE2 );
	glBindTexture( GL_TEXTURE_2D, targets[TARGET_NORMAL] );
	AGLOG( "GBuffer(beginPointLightPass)" );

	pointLightPass.setInt( pointLightDiffuseTarget, 0 );
	pointLightPass.setInt( pointLightPositionTarget, 1 );
	pointLightPass.setInt( pointLightNormalTarget, 2 );
	AGLOG( "GBuffer(beginPointLightPass)" );
}

void GBuffer::endPointLightPass()
{
	glDepthMask( GL_TRUE );
	glEnable( GL_DEPTH_TEST );
	glDisable( GL_BLEND );
	glCullFace( GL_BACK );
}

void GBuffer::renderPointLight( const PointLight& light )
{
	pointLightPass.setVec3( pointLightPosition, light.position );
	pointLightPass.setVec3( pointLightColor, light.color );
	pointLightPass.setFloat( pointLightIntensity, light.intensity );
	AGLOG( "GBuffer(renderPointLight)" );

	// TEMP: Magic numbers
	pointLightPass.setFloat( pointLightLinear, 1.0f );
	pointLightPass.setFloat( pointLightConstant, 0.0f );
	pointLightPass.setFloat( pointLightExponent, 1.0f );
	AGLOG( "GBuffer(renderPointLight)" );

	// Distance-from-attenuation formula:
	// http://ogldev.atspace.co.uk/www/tutorial36/tutorial36.html
	float C = fmax( fmax( light.color.r, light.color.g ), light.color.b );
	float radius = ( -light.linear + sqrt( powf( light.linear, 2.0f ) - 4*light.exponent * ( light.constant - 256*C*light.intensity ) ) ) / (2*light.exponent);

	glm::mat4 worldMatrix = glm::scale( glm::translate( glm::mat4(), light.position ), glm::vec3( radius ) );
	pointLightPass.setMat4( pointLightWorldMatrix, worldMatrix );

	Mesh* sphere = assets->getMesh( sphereMesh );
	assert( sphere );

	sphere->render(1);
	AGLOG( "GBuffer(renderPointLight)" );
}

void GBuffer::beginBillboardPass( Camera* camera )
{
	/*const int NUM_BUFFERS = 4;
	GLenum drawBuffers[NUM_BUFFERS] =
	{
		GL_COLOR_ATTACHMENT0+TARGET_DIFFUSE,
		GL_COLOR_ATTACHMENT0+TARGET_POSITION,
		GL_COLOR_ATTACHMENT0+TARGET_NORMAL,
		GL_COLOR_ATTACHMENT0+TARGET_ALPHA,
	};
	glDrawBuffers( NUM_BUFFERS, drawBuffers );

	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glDisable( GL_DEPTH_TEST );

	billboardPass.bind();
	AGLOG( "GBuffer(beginBillboardPass)" );

	billboardPass.setMat4( billboardProjectionMatrix, camera->getFinalProjectionMatrix() );
	billboardPass.setMat4( billboardViewMatrix, camera->getFinalViewMatrix() );
	// TEMP: Magic numbers
	billboardPass.setVec2( billboardScreenSize, glm::vec2( 640.0f, 480.0f ) );
	AGLOG( "GBuffer(beginBillboardPass) ");

	glActiveTexture( GL_TEXTURE3 );
	glBindTexture( GL_TEXTURE_2D, targets[TARGET_DEPTH] );

	billboardPass.setInt( billboardDiffuseMap, 0 );
	billboardPass.setInt( billboardNormalMap, 1 );
	billboardPass.setInt( billboardSpecularMap, 2 );
	billboardPass.setInt( billboardDepthTarget, 3 );
	AGLOG( "GBuffer(beginBillboardPass)" );*/

	glDrawBuffer( GL_COLOR_ATTACHMENT0+TARGET_ALPHA );
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT );

	const int NUM_BUFFERS = 2;
	GLenum drawBuffers[NUM_BUFFERS] =
	{
		GL_COLOR_ATTACHMENT0+TARGET_BILLBOARD,
		GL_COLOR_ATTACHMENT0+TARGET_ALPHA
	};
	glDrawBuffers( NUM_BUFFERS, drawBuffers );

	glClear( GL_DEPTH_BUFFER_BIT );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	//glDisable( GL_DEPTH_TEST );

	billboardPass.bind();
	AGLOG( "GBuffer(beginBillboardPass)" );

	billboardPass.setMat4( billboardProjectionMatrix, camera->getFinalProjectionMatrix() );
	billboardPass.setMat4( billboardViewMatrix, camera->getFinalViewMatrix() );
	// TEMP: Magic numbers
	billboardPass.setVec2( billboardScreenSize, glm::vec2( 640.0f, 480.0f ) );
	AGLOG( "GBuffer(beginBillboardPass) ");

	glActiveTexture( GL_TEXTURE3 );
	glBindTexture( GL_TEXTURE_2D, targets[TARGET_DEPTH] );

	billboardPass.setInt( billboardDiffuseMap, 0 );
	billboardPass.setInt( billboardNormalMap, 1 );
	billboardPass.setInt( billboardSpecularMap, 2 );
	billboardPass.setInt( billboardDepthTarget, 3 );
	AGLOG( "GBuffer(beginBillboardPass)" );
}

void GBuffer::endBillboardPass()
{
	glEnable( GL_DEPTH_TEST );
	glDisable( GL_BLEND );
}

void GBuffer::renderBillboards( Array<Billboard>* billboards )
{
	glBindVertexArray( billboardVAO );
	glBindBuffer( GL_ARRAY_BUFFER, billboardVBO );

	const int SIZE = sizeof(Billboard) * billboards->getSize();
	glBufferData( GL_ARRAY_BUFFER, SIZE, billboards->getData(), GL_DYNAMIC_DRAW );

	glDrawArrays( GL_POINTS, 0, billboards->getSize() );
	glBindVertexArray( 0 );
}

void GBuffer::performFinalPass()
{
	glDisable( GL_DEPTH_TEST );
	glDrawBuffer( GL_COLOR_ATTACHMENT0+TARGET_FINAL );

	finalPass.bind();
	AGLOG( "GBuffer(performFinalPass)" );

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, targets[TARGET_LIGHT] );
	glActiveTexture( GL_TEXTURE1 );
	glBindTexture( GL_TEXTURE_2D, targets[TARGET_BILLBOARD] );
	glActiveTexture( GL_TEXTURE2 );
	glBindTexture( GL_TEXTURE_2D, targets[TARGET_ALPHA] );
	AGLOG( "GBuffer(performFinalPass)" );

	finalPass.setInt( finalLightTarget, 0 );
	finalPass.setInt( finalBillboardTarget, 1 );
	finalPass.setInt( finalBillboardAlphaTarget, 2 );
	AGLOG( "GBuffer(performFinalPass)" );

	glBindVertexArray( quadVAO );
	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
	glBindVertexArray( 0 );
	AGLOG( "GBuffer(performFinalPass)" );

	glEnable( GL_DEPTH_TEST );
}

void GBuffer::setDebugMode( int mode )
{
	debugMode = mode;
}

void GBuffer::toggleDebugMode()
{
	debugMode = ( debugMode + 1 ) % MAX_DEBUG_MODES;
}

GLuint GBuffer::getFBO() const
{
	return fbo;
}

GLuint GBuffer::getTarget( int index ) const
{
	assert( index >= 0 && index < MAX_TARGETS );
	return targets[index];
}
