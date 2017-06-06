#include "Mesh.h"

Mesh::Mesh()
	: vertices( nullptr ), indices( nullptr ),
	vao( 0 ), vbo( 0 ), ibo( 0 ),
	vertexCount( 0 ), indexCount( 0 )
{
}

Mesh::Mesh( const Mesh& ref )
	: vertices( ref.vertices ), indices( ref.indices ),
	vao( ref.vao ), vbo( ref.vbo ), ibo( ref.ibo ),
	vertexCount( ref.vertexCount ), indexCount( ref.indexCount )
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

	return *this;
}

bool Mesh::load( const char* path )
{
	bool result = false;

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

		result = true;
	}

	return result;
}

void Mesh::upload()
{
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
	glVertexAttribPointer( 3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(GLfloat)*8) );
	glVertexAttribPointer( 4, 5, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(GLfloat)*11) );

	glBindVertexArray( 0 );

	delete[] vertices;
	delete[] indices;

	vertices = nullptr;
	indices = nullptr;
}

void Mesh::unload()
{
	if( vao > 0 )
	{
		glBindVertexArray( vao );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

		glDeleteBuffers( 1, &vbo );
		glDeleteBuffers( 1, &ibo );
		glDeleteVertexArrays( 1, &vao );
	}

	vbo = ibo = vao = 0;
	vertexCount = indexCount = 0;
}

void Mesh::render( int instances ) const
{
	assert( instances > 0 );

	glBindVertexArray( vao );
	glDrawElementsInstanced( GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, NULL, instances );
}

void Mesh::bind() const
{
	glBindVertexArray( vao );
}