#include "Shader.h"

Shader::Shader()
	: program( 0 )
{
}

Shader::~Shader()
{
	unload();
}

bool Shader::load( const char* vertex, const char* geometry, const char* fragment )
{
	if( vertex )
	{
		vertexData = readFile( vertex );
	}

	if( geometry )
	{
		geometryData = readFile( geometry );
	}

	if( fragment )
	{
		fragmentData = readFile( fragment );
	}

	return ( vertexData || geometryData || fragmentData );
}

void Shader::upload()
{
	assert( vertexData || geometryData || fragmentData );

	if( program > 0 )
	{
		glDeleteProgram( program );
	}

	program = glCreateProgram();

	if( vertexData )
	{
		compile( GL_VERTEX_SHADER, vertexData );
		delete[] vertexData;
	}

	if( geometryData )
	{
		compile( GL_GEOMETRY_SHADER, geometryData );
		delete[] geometryData;
	}

	if( fragmentData )
	{
		compile( GL_FRAGMENT_SHADER, fragmentData );
		delete[] fragmentData;
	}

	link();
}

void Shader::unload()
{
	if( program > 0 )
	{
		glDeleteProgram( program );
	}

	if( vertexData )
	{
		delete[] vertexData;
	}

	if( geometryData )
	{
		delete[] geometryData;
	}

	if( fragmentData )
	{
		delete[] fragmentData;
	}
}

void Shader::setInt( GLuint uniform, int* values, int count )
{
	glUniform1iv( uniform, count, values );
}

void Shader::setFloat( GLuint uniform, float* values, int count )
{
	glUniform1fv( uniform, count, values );
}

void Shader::setVec2( GLuint uniform, glm::vec2& value )
{
	glUniform2f( uniform, value.x, value.y );
}

void Shader::setVec3( GLuint uniform, glm::vec3& value )
{
	glUniform3f( uniform, value.x, value.y, value.z );
}

void Shader::setVec4( GLuint uniform, glm::vec4& value )
{
	glUniform4f( uniform, value.x, value.y, value.z, value.w );
}

void Shader::setMat4( GLuint uniform, glm::mat4* values, int count )
{
	glUniformMatrix4fv( uniform, count, GL_FALSE, glm::value_ptr(*values) );
}

GLuint Shader::getUniform( const char* name )
{
	return glGetUniformLocation( program, name );
}

char* Shader::readFile( const char* path )
{
	char* result = nullptr;

	FILE* file = fopen( path, "rb" );
	if( file )
	{
		fseek( file, 0, SEEK_END );
		int len = ftell( file );
		fseek( file, 0, SEEK_SET );

		result = new char[len+1];
		fread( result, 1, len, file );
		result[len] = 0;

		fclose( file );
	}

	return result;
}

void Shader::compile( GLenum type, const char* data )
{
	GLuint shader = glCreateShader( type );
	glShaderSource( shader, 1, &data, 0 );
	glCompileShader( shader );
	glAttachShader( program, shader );
	glDeleteShader( shader );
}

void Shader::link()
{
	glLinkProgram( program );
}