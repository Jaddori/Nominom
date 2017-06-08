#include "Shader.h"

Shader::Shader()
	: program( 0 ), valid( false )
{
}

Shader::~Shader()
{
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

	valid = ( vertexData || geometryData || fragmentData );
	return valid;
}

void Shader::upload()
{
	assert( valid && ( vertexData || geometryData || fragmentData ) );

	if( program > 0 )
	{
		glDeleteProgram( program );
	}

	program = glCreateProgram();

	if( vertexData )
	{
		compile( GL_VERTEX_SHADER, vertexData );
		delete[] vertexData;
		vertexData = nullptr;
	}

	if( geometryData )
	{
		compile( GL_GEOMETRY_SHADER, geometryData );
		delete[] geometryData;
		geometryData = nullptr;
	}

	if( fragmentData )
	{
		compile( GL_FRAGMENT_SHADER, fragmentData );
		delete[] fragmentData;
		fragmentData = nullptr;
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

	valid = false;
}

void Shader::bind()
{
	assert( valid );
	glUseProgram( program );
}

void Shader::setInt( GLuint uniform, const int* values, int count )
{
	assert( valid );
	glUniform1iv( uniform, count, values );
}

void Shader::setFloat( GLuint uniform, const float* values, int count )
{
	assert( valid );
	glUniform1fv( uniform, count, values );
}

void Shader::setVec2( GLuint uniform, const glm::vec2& value )
{
	assert( valid );
	glUniform2f( uniform, value.x, value.y );
}

void Shader::setVec3( GLuint uniform, const glm::vec3& value )
{
	assert( valid );
	glUniform3f( uniform, value.x, value.y, value.z );
}

void Shader::setVec4( GLuint uniform, const glm::vec4& value )
{
	assert( valid );
	glUniform4f( uniform, value.x, value.y, value.z, value.w );
}

void Shader::setMat4( GLuint uniform, const glm::mat4* values, int count )
{
	assert( valid );
	glUniformMatrix4fv( uniform, count, GL_FALSE, glm::value_ptr(*values) );
}

GLint Shader::getUniform( const char* name )
{
	assert( valid );
	return glGetUniformLocation( program, name );
}

bool Shader::getValid() const
{
	return valid;
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
	else
	{
		valid = false;
	}

	return result;
}

void Shader::compile( GLenum type, const char* data )
{
	GLuint shader = glCreateShader( type );
	glShaderSource( shader, 1, &data, 0 );
	glCompileShader( shader );

	GLint success = 0;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
	if( success != GL_TRUE )
	{
		char buffer[256] = {};
		int len = 256;
		glGetShaderInfoLog( shader, 256, &len, buffer );
		buffer[255] = 0;

		printf( "%s\n", buffer );
		valid = false;

		assert( false );
	}

	glAttachShader( program, shader );
	glDeleteShader( shader );
}

void Shader::link()
{
	glLinkProgram( program );

	GLint success = 0;
	glGetProgramiv( program, GL_LINK_STATUS, &success );
	if( success != GL_TRUE )
	{
		char buffer[256] = {};
		int len = 256;
		glGetProgramInfoLog( program, 256, &len, buffer );
		buffer[255] = 0;

		printf( "%s\n", buffer );
		valid = false;

		assert( false );
	}
}