#include "Model.h"

Model::Model()
	: valid( false )
{
}

Model::Model( const Model& ref )
	: meshes( ref.meshes ), textures( ref.textures ), valid( ref.valid )
{
}

Model::~Model()
{
}

Model& Model::operator=( const Model& ref )
{
	meshes = ref.meshes;
	textures = ref.textures;
	valid = ref.valid;

	return *this;
}

bool Model::load( const char* path )
{
	FILE* file = fopen( path, "rb" );
	if( file )
	{
		fseek( file, 0, SEEK_END );
		int len = ftell( file );
		fseek( file, 0, SEEK_SET );

		char* text = new char[len];

		// TODO: Implement
		assert( false );

		delete[] text;

		fclose( file );

		valid = false;
	}
	else
	{
		LOG( VERBOSITY_ERROR, "Model", "Failed to open file %s.", path );
	}

	return valid;
}

bool Model::getValid() const
{
	return valid;
}