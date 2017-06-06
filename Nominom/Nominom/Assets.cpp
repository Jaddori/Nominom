#include "Assets.h"

Assets::Assets()
{
}

Assets::~Assets()
{
}

void Assets::upload()
{
	const int MAX_MESHES = uploadMeshes.getSize();
	for( int curMesh = 0; curMesh < MAX_MESHES; curMesh++ )
	{
		uploadMeshes[curMesh]->upload();
	}

	const int MAX_TEXTURES = uploadTextures.getSize();
	for( int curTexture = 0; curTexture < MAX_TEXTURES; curTexture++ )
	{
		uploadTextures[curTexture]->upload();
	}

	uploadMeshes.clear();
	uploadTextures.clear();
}

void Assets::unload()
{
}

int Assets::loadMesh( const char* path )
{
	int result = find( path, meshPaths );

	if( result < 0 )
	{
		Mesh mesh;
		if( mesh.load( path ) )
		{
			result = meshes.getSize();
			meshes.add( mesh );
			meshPaths.add( makeString( path ) );

			uploadMeshes.add( &meshes[result] );
		}
	}

	return result;
}

int Assets::loadTexture( const char* path )
{
	int result = find( path, texturePaths );

	if( result < 0 )
	{
		Texture texture;
		if( texture.load( path ) )
		{
			result = textures.getSize();
			textures.add( texture );
			texturePaths.add( makeString( path ) );

			uploadTextures.add( &textures[result] );
		}
	}

	return result;
}

int Assets::loadModel( const char* path )
{
	int result = find( path, modelPaths );

	if( result < 0 )
	{
		Model model;
		if( model.load( path ) )
		{
			result = models.getSize();
			models.add( model );
			modelPaths.add( makeString( path ) );
		}
	}

	return result;
}

Font* Assets::loadFont( const char* path )
{
	Font* result = nullptr;

	int index = find( path, fontPaths );
	if( index < 0 )
	{
		result = new Font();
		if( result->load( path ) )
		{
			delete result;
			result = nullptr;
		}
	}
	else
		result = &fonts[index];

	return result;
}

Mesh* Assets::getMesh( int index )
{
	assert( index >= 0 && index < meshes.getSize() );
	return &meshes[index];
}

Texture* Assets::getTexture( int index )
{
	assert( index >= 0 && index < textures.getSize() );
	return &textures[index];
}

Model* Assets::getModel( int index )
{
	assert( index >= 0 && index < models.getSize() );
	return &models[index];
}

int Assets::find( const char* path, Array<char*>& list )
{
	int result = -1;
	for( int i=0; i<list.getSize() && result < 0; i++ )
	{
		if( strcmp( path, list[i] ) == 0 )
		{
			result = i;
		}
	}

	return result;
}

char* Assets::makeString( const char* ref )
{
	int len = strlen( ref )+1;
	char* result = new char[len];
	strcpy( result, ref );
	result[len] = 0;

	return result;
}