#include "DebugShapes.h"

DebugShapes::DebugShapes()
{
}

DebugShapes::~DebugShapes()
{
}

bool DebugShapes::load()
{
	bool result = false;

	if( !sphereShader.load( "./assets/shaders/debug_sphere.vs", "./assets/shaders/debug_sphere.gs", "./assets/shaders/debug_shape.fs" ) )
	{
		LOG( "DebugShapes", "Failed to load debug sphere shader.", VERBOSITY_ERROR );
		result = false;
	}

	if( !aabbShader.load( "./assets/shaders/debug_aabb.vs", "./assets/shaders/debug_aabb.gs", "./assets/shaders/debug_shape.fs" ) )
	{
		LOG( "DebugShapes", "Failed to load debug aabb shader.", VERBOSITY_ERROR );
		result = false;
	}

	if( !obbShader.load( "./assets/shaders/debug_obb.vs", "./assets/shaders/debug_obb.gs", "./assets/shaders/debug_shape.fs" ) )
	{
		LOG( "DebugShapes", "Failed to load debug obb shader.", VERBOSITY_ERROR );
		result = false;
	}

	if( !lineShader.load( "./assets/shaders/debug_line.vs", "./assets/shaders/debug_line.gs", "./assets/shaders/debug_shape.fs" ) )
	{
		LOG( "DebugShapes", "Failed to load debug line shader.", VERBOSITY_ERROR );
		result = false;
	}

	return result;
}

void DebugShapes::upload()
{
	if( sphereShader.getValid() )
	{
		sphereShader.upload();
		sphereProjectionMatrix = sphereShader.getUniform( "projectionMatrix" );
		sphereViewMatrix = sphereShader.getUniform( "viewMatrix" );

		glGenVertexArrays( 1, &sphereVAO );
		glBindVertexArray( sphereVAO );

		glEnableVertexAttribArray( 0 );
		glEnableVertexAttribArray( 1 );
		glEnableVertexAttribArray( 2 );

		glGenBuffers( 1, &sphereVBO );
		glBindBuffer( GL_ARRAY_BUFFER, sphereVBO );

		const int STRIDE = sizeof(GLfloat)*8;
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, STRIDE, 0 );
		glVertexAttribPointer( 1, 1, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(GLfloat)*3) );
		glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(GLfloat)*4) );

		glBindVertexArray( 0 );
	}

	if( aabbShader.getValid() )
	{
		aabbShader.upload();
		aabbProjectionMatrix = aabbShader.getUniform( "projectionMatrix" );
		aabbViewMatrix = aabbShader.getUniform( "viewMatrix" );

		glGenVertexArrays( 1, &aabbVAO );
		glBindVertexArray( aabbVAO );

		glEnableVertexAttribArray( 0 );
		glEnableVertexAttribArray( 1 );
		glEnableVertexAttribArray( 2 );

		glGenBuffers( 1, &aabbVBO );
		glBindBuffer( GL_ARRAY_BUFFER, aabbVBO );

		const int STRIDE = sizeof(GLfloat)*10;
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, STRIDE, 0 );
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(GLfloat)*3) );
		glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(GLfloat)*6) );

		glBindVertexArray( 0 );
	}

	if( obbShader.getValid() )
	{
		obbShader.upload();
		obbProjectionMatrix = obbShader.getUniform( "projectionMatrix" );
		obbViewMatrix = obbShader.getUniform( "viewMatrix" );

		glGenVertexArrays( 1, &obbVAO );
		glBindVertexArray( obbVAO );

		glEnableVertexAttribArray( 0 );
		glEnableVertexAttribArray( 1 );
		glEnableVertexAttribArray( 2 );
		glEnableVertexAttribArray( 3 );
		glEnableVertexAttribArray( 4 );
		glEnableVertexAttribArray( 5 );

		glGenBuffers( 1, &obbVBO );
		glBindBuffer( GL_ARRAY_BUFFER, obbVBO );

		const int STRIDE = sizeof(GLfloat)*19;
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, STRIDE, 0 );
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(GLfloat)*3) );
		glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(GLfloat)*6) );
		glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(GLfloat)*9) );
		glVertexAttribPointer( 4, 3, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(GLfloat)*12) );
		glVertexAttribPointer( 5, 4, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(GLfloat)*15) );

		glBindVertexArray( 0 );
	}

	if( lineShader.getValid() )
	{
		lineShader.upload();
		lineProjectionMatrix = lineShader.getUniform( "projectionMatrix" );
		lineViewMatrix = lineShader.getUniform( "viewMatrix" );

		glGenVertexArrays( 1, &lineVAO );
		glBindVertexArray( lineVAO );

		glEnableVertexAttribArray( 0 );
		glEnableVertexAttribArray( 1 );
		glEnableVertexAttribArray( 2 );

		glGenBuffers( 1, &lineVBO );
		glBindBuffer( GL_ARRAY_BUFFER, lineVBO );

		const int STRIDE = sizeof(GLfloat)*10;
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, STRIDE, 0 );
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(GLfloat)*3) );
		glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(GLfloat)*6) );

		glBindVertexArray( 0 );
	}
}

void DebugShapes::render( Camera* camera )
{
	assert( sphereShader.getValid() &&
			aabbShader.getValid() &&
			obbShader.getValid() &&
			lineShader.getValid() );

	const glm::mat4* projectionMatrix = &camera->getFinalProjectionMatrix();
	const glm::mat4* viewMatrix = &camera->getFinalViewMatrix();
	
	const int NUM_SPHERES = finalSpheres.getSize();
	const int NUM_AABB = finalAABBs.getSize();
	const int NUM_OBB = finalOBBs.getSize();
	const int NUM_LINES = finalLines.getSize();

	// sphere
	if( NUM_SPHERES > 0 )
	{
		glBindVertexArray( sphereVAO );
		glBindBuffer( GL_ARRAY_BUFFER, sphereVBO );
		glBufferData( GL_ARRAY_BUFFER, sizeof(DebugSphere)*NUM_SPHERES, finalSpheres.getData(), GL_DYNAMIC_DRAW );

		sphereShader.bind();
		sphereShader.setMat4( sphereProjectionMatrix, projectionMatrix, 1 );
		sphereShader.setMat4( sphereViewMatrix, viewMatrix, 1 );

		glDrawArrays( GL_POINTS, 0, NUM_SPHERES );
	}

	// aabb
	if( NUM_AABB > 0 )
	{
		glBindVertexArray( aabbVAO );
		glBindBuffer( GL_ARRAY_BUFFER, aabbVBO );
		glBufferData( GL_ARRAY_BUFFER, sizeof(DebugAABB)*NUM_AABB, finalAABBs.getData(), GL_DYNAMIC_DRAW );

		aabbShader.bind();
		aabbShader.setMat4( aabbProjectionMatrix, projectionMatrix, 1 );
		aabbShader.setMat4( aabbViewMatrix, viewMatrix, 1 );

		glDrawArrays( GL_POINTS, 0, NUM_AABB );
	}

	// obb
	if( NUM_OBB > 0 )
	{
		glBindVertexArray( obbVAO );
		glBindBuffer( GL_ARRAY_BUFFER, obbVBO );
		glBufferData( GL_ARRAY_BUFFER, sizeof(DebugOBB)*NUM_OBB, finalOBBs.getData(), GL_DYNAMIC_DRAW );

		obbShader.bind();
		obbShader.setMat4( obbProjectionMatrix, projectionMatrix, 1 );
		obbShader.setMat4( obbViewMatrix, viewMatrix, 1 );

		glDrawArrays( GL_POINTS, 0, NUM_OBB );
	}
	
	// line
	if( NUM_LINES > 0 )
	{
		glBindVertexArray( lineVAO );
		glBindBuffer( GL_ARRAY_BUFFER, lineVBO );
		glBufferData( GL_ARRAY_BUFFER, sizeof(DebugLine)*NUM_LINES, finalLines.getData(), GL_DYNAMIC_DRAW );

		lineShader.bind();
		lineShader.setMat4( lineProjectionMatrix, projectionMatrix, 1 );
		lineShader.setMat4( lineViewMatrix, viewMatrix, 1 );

		glDrawArrays( GL_POINTS, 0, NUM_LINES );
	}

	// reset
	glBindVertexArray( 0 );

	finalSpheres.clear();
	finalAABBs.clear();
	finalOBBs.clear();
	finalLines.clear();
}

void DebugShapes::finalize()
{
	finalSpheres.fastCopy( spheres );
	finalAABBs.fastCopy( aabbs );
	finalOBBs.fastCopy( obbs );
	finalLines.fastCopy( lines );

	spheres.clear();
	aabbs.clear();
	obbs.clear();
	lines.clear();
}

void DebugShapes::addSphere( const DebugSphere& sphere )
{
	spheres.add( sphere );
}

void DebugShapes::addAABB( const DebugAABB& aabb )
{
	aabbs.add( aabb );
}

void DebugShapes::addOBB( const DebugOBB& obb )
{
	obbs.add( obb );
}

void DebugShapes::addLine( const DebugLine& line )
{
	lines.add( line );
}