#include "Model.h"

Model::Model()
{
}

Model::Model( const Model& ref )
	: meshes( ref.meshes ), textures( ref.textures )
{
}

Model::~Model()
{
}

Model& Model::operator=( const Model& ref )
{
	meshes = ref.meshes;
	textures = ref.textures;

	return *this;
}

bool Model::load( const char* path )
{
	bool result = false;

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

		result = false;
	}

	return result;
}