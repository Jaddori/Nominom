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
	void bind();
	
	void setInt( GLuint uniform, const int* values, int count );
	void setInt( GLuint uniform, int value );
	void setFloat( GLuint uniform, const float* values, int count );
	void setFloat( GLuint uniform, float value );
	void setVec2( GLuint uniform, const glm::vec2& value );
	void setVec3( GLuint uniform, const glm::vec3& value );
	void setVec4( GLuint uniform, const glm::vec4& value );
	void setMat4( GLuint uniform, const glm::mat4* values, int count );
	void setMat4( GLuint uniform, const glm::mat4& value );

	GLint getUniform( const char* name );
	bool getValid() const;

private:
	char* readFile( const char* path );
	void compile( GLenum type, const char* data );
	void link();

	GLuint program;
	char* vertexData;
	char* geometryData;
	char* fragmentData;

	bool valid;
};