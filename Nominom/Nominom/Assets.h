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

	void upload();
	void unload();

	int loadMesh( const char* path );
	int loadTexture( const char* path );
	int loadModel( const char* path );
	Font* loadFont( const char* path );

	Mesh* getMesh( int index );
	Texture* getTexture( int index );
	Model* getModel( int index );

private:
	int find( const char* path, Array<char*>& list );
	char* makeString( const char* ref );

	Assets( const Assets& ref ){}
	Assets& operator=( const Assets& ref ){ return *this; }

	Array<Mesh> meshes;
	Array<Texture> textures;
	Array<Model> models;
	Array<Font> fonts;

	Array<char*> meshPaths;
	Array<char*> texturePaths;
	Array<char*> modelPaths;
	Array<char*> fontPaths;

	Array<Mesh*> uploadMeshes;
	Array<Texture*> uploadTextures;
};
