#pragma once

#include "BaseIncludes.h"
#include "Vertex.h"

class Mesh
{
public:
	Mesh();
	Mesh( const Mesh& ref );
	~Mesh();

	Mesh& operator=( const Mesh& ref );

	bool load( const char* path );
	void upload();
	void unload();
	void render( int instances ) const;
	void bind() const;

private:
	Vertex* vertices;
	GLuint* indices;
	GLuint vao, vbo, ibo;
	int vertexCount, indexCount;
};