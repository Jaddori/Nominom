#pragma once

#include "BaseIncludes.h"

class Shader
{
public:
	Shader();
	~Shader();

	bool load( const char* vertex, const char* geometry, const char* fragment );
	void upload();
	void unload();

	void setInt( GLuint uniform, int* values, int count );
	void setFloat( GLuint uniform, float* values, int count );
	void setVec2( GLuint uniform, glm::vec2& value );
	void setVec3( GLuint uniform, glm::vec3& value );
	void setVec4( GLuint uniform, glm::vec4& value );
	void setMat4( GLuint uniform, glm::mat4* values, int count );

	GLuint getUniform( const char* name );

private:
	char* readFile( const char* path );
	void compile( GLenum type, const char* data );
	void link();

	GLuint program;
	char* vertexData;
	char* geometryData;
	char* fragmentData;
};