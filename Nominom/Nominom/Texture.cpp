#include "Texture.h"

Texture::Texture()
	: id( 0 ), pixels( nullptr ),
	width( 0 ), height( 0 ),
	format( 0 ), size( 0 ),
	valid( false )
{
}

Texture::Texture( const Texture& ref )
	: id( ref.id ), pixels( ref.pixels ),
	width( ref.width ), height( ref.height ),
	format( ref.format ), size( ref.size ),
	valid( ref.valid )
{
}

Texture::~Texture()
{
}

Texture& Texture::operator=( const Texture& ref )
{
	id = ref.id;
	pixels = ref.pixels;
	width = ref.width;
	height = ref.height;
	format = ref.format;
	size = ref.size;
	valid = ref.valid;

	return *this;
}

bool Texture::load( const char* path )
{
	FILE* file = fopen( path, "rb" );
	if( file )
	{
		int32_t magicNumber;
		fread( &magicNumber, sizeof(magicNumber), 1, file );

		if( magicNumber == DDS_MAGIC_NUMBER )
		{
			DDS_HEADER header;
			fread( &header, sizeof(header), 1, file );

			width = header.width;
			height = header.height;
			size = header.pitch;

			switch( header.format.fourCC )
			{
			default:
			case ID_DXT1: format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; break;
			case ID_DXT3: format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; break;
			case ID_DXT5: format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; break;
			}

			pixels = new GLbyte[size];
			fread( pixels, sizeof( GLbyte ), size, file );

			valid = true;
		}

		fclose( file );
	}
	else
	{
		LOG( VERBOSITY_ERROR, "Texture", "Failed to open file %s.", path );
	}

	return valid;
}

void Texture::upload()
{
	assert( valid && pixels );

	GLOG( "Texture" );

	glGenTextures( 1, &id );
	glBindTexture( GL_TEXTURE_2D, id );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glCompressedTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, size, pixels );
	glBindTexture( GL_TEXTURE_2D, 0 );

	delete[] pixels;
	pixels = nullptr;

	GLOG( "Texture" );
}

void Texture::unload()
{
	if( id )
	{
		GLOG( "Texture" );

		glDeleteTextures( 1, &id );

		GLOG( "Texture" );
	}

	id = 0;
	width = height = 0;

	if( pixels )
	{
		delete[] pixels;
		pixels = nullptr;
	}

	valid = false;
}

void Texture::bind( GLenum location ) const
{
	assert( valid );

	// DEBUG: Remove or surround with #ifdef _DEBUG
	//GLOG( "Texture" );
	GLenum glError = glGetError();
	if( glError )
	{
		LOG( VERBOSITY_WARNING, "Texture", "OpenGL error: %d", glError );
	}

	glActiveTexture( location );
	glBindTexture( GL_TEXTURE_2D, id );

	//GLOG( "Texture" );
	glError = glGetError();
	if( glError )
	{
		LOG( VERBOSITY_WARNING, "Texture", "OpenGL error: %d", glError );
	}
}

GLuint Texture::getID() const
{
	return id;
}

int Texture::getWidth() const
{
	return width;
}

int Texture::getHeight() const
{
	return height;
}

bool Texture::getValid() const
{
	return valid;
}