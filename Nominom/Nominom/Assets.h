#pragma once

#include "Mesh.h"
#include "Texture.h"
#include "Model.h"
#include "Font.h"

class Assets
{
public:
	Assets();
	~Assets();

	int loadMesh( const char* path );
	int loadTexture( const char* path );
	int loadModel( const char* path );
	Font* loadFont( const char* path );

private:
	int find( const char* path, Array<char*>& list );
	char* makeString( const char* ref );

	Assets( const Assets& ref ){}
	Assets& operator=( const Assets& ref ){}

	Array<Mesh> meshes;
	Array<Texture> textures;
	Array<Model> models;
	Array<Font> fonts;

	Array<char*> meshPaths;
	Array<char*> texturePaths;
	Array<char*> modelPaths;
	Array<char*> fontPaths;
};