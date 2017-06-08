#include "Assets.h"

Assets::Assets()
{
	LOG( VERBOSITY_INFORMATION, "Assets", "Constructing." );
}

Assets::~Assets()
{
	LOG( VERBOSITY_INFORMATION, "Assets", "Destructing." );
}

void Assets::upload()
{
	LOG( VERBOSITY_INFORMATION, "Assets", "Uploading assets." );

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
	LOG( VERBOSITY_INFORMATION, "Assets", "Unloading assets." );
}

int Assets::loadMesh( const char* path )
{
	LOG( VERBOSITY_INFORMATION, "Assets", "Loading mesh %s.", path );

	int result = find( path, meshPaths );

	if( result < 0 )
	{
		LOG( VERBOSITY_INFORMATION, "Assets", "Mesh not found. Loading from file." );

		Mesh mesh;
		if( mesh.load( path ) )
		{
			result = meshes.getSize();
			meshes.add( mesh );
			meshPaths.add( makeString( path ) );

			uploadMeshes.add( &meshes[result] );
		}
		else
		{
			LOG( VERBOSITY_ERROR, "Assets", "Failed to load mesh %s.", path );
		}
	}
	else
	{
		LOG( VERBOSITY_INFORMATION, "Assets", "Found mesh at index %d.", result );
	}

	return result;
}

int Assets::loadTexture( const char* path )
{
	LOG( VERBOSITY_INFORMATION, "Assets", "Loading texture %s.", path );
	
	int result = find( path, texturePaths );

	if( result < 0 )
	{
		LOG( VERBOSITY_INFORMATION, "Assets", "Texture not found. Loading from file." );

		Texture texture;
		if( texture.load( path ) )
		{
			result = textures.getSize();
			textures.add( texture );
			texturePaths.add( makeString( path ) );

			uploadTextures.add( &textures[result] );
		}
		else
		{
			LOG( VERBOSITY_ERROR, "Assets", "Failed to load texture %s.", path );
		}
	}
	else
	{
		LOG( VERBOSITY_INFORMATION, "Assets", "Found texture at index %d.", result );
	}

	return result;
}

int Assets::loadModel( const char* path )
{
	LOG( VERBOSITY_INFORMATION, "Assets", "Loading model %s.", path );

	int result = find( path, modelPaths );

	if( result < 0 )
	{
		LOG( VERBOSITY_INFORMATION, "Assets", "Model not found. Loading from file." );

		Model model;
		if( model.load( path ) )
		{
			result = models.getSize();
			models.add( model );
			modelPaths.add( makeString( path ) );
		}
		else
		{
			LOG( VERBOSITY_ERROR, "Assets", "Failed to load model %s.", path );
		}
	}
	else
	{
		LOG( VERBOSITY_INFORMATION, "Assets", "Found model at index %d.", result );
	}

	return result;
}

Font* Assets::loadFont( const char* path )
{
	LOG( VERBOSITY_INFORMATION, "Assets", "Loading font %s.", path );

	Font* result = nullptr;

	int index = find( path, fontPaths );
	if( index < 0 )
	{
		LOG( VERBOSITY_INFORMATION, "Assets", "Font not found. Loading from file." );

		result = new Font();
		if( result->load( path ) )
		{
			delete result;
			result = nullptr;
		}
		else
		{
			LOG( VERBOSITY_ERROR, "Assets", "Failed to load font %s.", path );
		}
	}
	else
	{
		result = &fonts[index];
		LOG( VERBOSITY_INFORMATION, "Assets", "Font found at index %d.", result );
	}

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