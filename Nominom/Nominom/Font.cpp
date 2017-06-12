#include "Font.h"

Font::Font()
	: valid( false )
{
}

Font::Font( const Font& ref )
	: info( ref.info ), valid( false )
{
}

Font::~Font()
{
}

Font& Font::operator=( const Font& ref )
{
	info = ref.info;
	valid = ref.valid;

	return *this;
}

bool Font::load( const char* path )
{
	FILE* file = fopen( path, "rb" );
	if( file )
	{
		fread( &info, sizeof(info), 1, file );
		fclose( file );

		valid = true;
	}
	else
	{
		LOG( VERBOSITY_ERROR, "Font", "Failed to open file %s.", path );
	}

	return valid;
}

float Font::getHeight() const
{
	return info.height;
}

float Font::getTextureSize() const
{
	return info.textureSize;
}

float Font::getWidth( int index ) const
{
	return info.widths[index];
}

glm::vec2 Font::getSize( int index ) const
{
	return glm::vec2( info.widths[index], info.height );
}

glm::vec2 Font::getOffset( int index ) const
{
	return glm::vec2( info.xoffsets[index], info.yoffsets[index] );
}

glm::vec2 Font::getStartUV( int index ) const
{
	return glm::vec2( (float)info.xoffsets[index] / info.textureSize,
						(float)( info.yoffsets[index] - info.height ) / info.textureSize );
}

glm::vec2 Font::getEndUV( int index ) const
{
	return glm::vec2( (float)( info.xoffsets[index] + info.widths[index] ) / info.textureSize,
						(float)info.yoffsets[index] / info.textureSize );
}

const FontInfo& Font::getInfo() const
{
	return info;
}

bool Font::getValid() const
{
	return valid;
}