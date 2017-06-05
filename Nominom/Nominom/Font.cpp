#include "Font.h"

Font::Font()
{
}

Font::Font( const Font& ref )
	: info( ref.info )
{
}

Font::~Font()
{
}

Font& Font::operator=( const Font& ref )
{
	info = ref.info;

	return *this;
}

bool Font::load( const char* path )
{
	bool result = false;

	FILE* file = fopen( path, "rb" );
	if( file )
	{
		fread( &info, sizeof(info), 1, file );
		fclose( file );

		result = true;
	}

	return result;
}