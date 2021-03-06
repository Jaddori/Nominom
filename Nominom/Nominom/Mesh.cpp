#include "Mesh.h"

Mesh::Mesh()
	: vertices( nullptr ), indices( nullptr ),
	vao( 0 ), vbo( 0 ), ibo( 0 ),
	vertexCount( 0 ), indexCount( 0 ),
	valid( false )
{
}

Mesh::Mesh( const Mesh& ref )
	: vertices( ref.vertices ), indices( ref.indices ),
	vao( ref.vao ), vbo( ref.vbo ), ibo( ref.ibo ),
	vertexCount( ref.vertexCount ), indexCount( ref.indexCount ),
	valid( false )
{
}

Mesh::~Mesh()
{
}

Mesh& Mesh::operator=( const Mesh& ref )
{
	vertices = ref.vertices;
	indices = ref.indices;

	vao = ref.vao;
	vbo = ref.vbo;
	ibo = ref.ibo;

	vertexCount = ref.vertexCount;
	indexCount = ref.indexCount;

	valid = ref.valid;

	return *this;
}

bool Mesh::load( const char* path )
{
	FILE* file = fopen( path, "rb" );
	if( file )
	{
		fread( &vertexCount, sizeof(vertexCount), 1, file );
		fread( &indexCount, sizeof(indexCount), 1, file );

		vertices = new Vertex[vertexCount];
		indices = new GLuint[indexCount];

		fread( vertices, sizeof(Vertex), vertexCount, file );
		fread( indices, sizeof(GLuint), indexCount, file );

		fclose( file );

		valid = true;
	}
	else
	{
		LOG( VERBOSITY_ERROR, "Mesh", "Failed to open file %s.", path );
	}

	return valid;
}

void Mesh::upload()
{
	assert( valid );

	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	const int MAX_ATTRIB = 5;
	for( int i=0; i<MAX_ATTRIB; i++ )
	{
		glEnableVertexAttribArray( i );
	}

	glGenBuffers( 1, &vbo );
	glGenBuffers( 1, &ibo );

	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );

	glBufferData( GL_ARRAY_BUFFER, sizeof(Vertex)*vertexCount, vertices, GL_STATIC_DRAW );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indexCount, indices, GL_STATIC_DRAW );

	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0 );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(GLfloat)*3) );
	glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(GLfloat)*5) );
	glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(GLfloat)*8) );
	glVertexAttribPointer( 4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(GLfloat)*11) );

	glBindVertexArray( 0 );

	/*delete[] vertices;
	delete[] indices;

	vertices = nullptr;
	indices = nullptr;*/

	GLOG( "Mesh" );
}

void Mesh::unload()
{
	if( vao > 0 )
	{
		GLOG( "Mesh" );

		glBindVertexArray( vao );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

		glDeleteBuffers( 1, &vbo );
		glDeleteBuffers( 1, &ibo );
		glDeleteVertexArrays( 1, &vao );

		GLOG( "Mesh" );
	}

	vbo = ibo = vao = 0;
	vertexCount = indexCount = 0;

	valid = false;
}

void Mesh::render( int instances ) const
{
	assert( valid && instances > 0 );

	// DEBUG: Probably remove this or surround it with #ifdef _DEBUG
	GLOG( "Mesh" );

	glBindVertexArray( vao );
	glDrawElementsInstanced( GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, NULL, instances );
	glBindVertexArray( 0 );

	GLOG( "Mesh" );
}

void Mesh::bind() const
{
	assert( valid );
	glBindVertexArray( vao );
}

bool Mesh::getValid() const
{
	return valid;
}

const Vertex* Mesh::getVertices( int* count ) const
{
	*count = vertexCount;
	return vertices;
}