#pragma once

#include "Mesh.h"
#include "Texture.h"

class Model
{
public:
	Model();
	Model( const Model& ref );
	~Model();

	Model& operator=( const Model& ref );

	bool load( const char* path );

private:
	Array<int> meshes;
	Array<int> textures;
};