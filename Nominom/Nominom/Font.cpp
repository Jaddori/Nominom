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

bool Font::getValid() const
{
	return valid;
}