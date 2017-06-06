#pragma once

#include "BaseIncludes.h"

#define DDS_MAGIC_NUMBER 0x20534444 // 'DDS ' in hex
#define ID_DXT1 0x31545844
#define ID_DXT3 0x33545844
#define ID_DXT5 0x35545844

struct DDS_PIXELFORMAT
{
	int32_t size;
	int32_t flags;
	int32_t fourCC;
	int32_t bitCount;
	int32_t masks[4];
};

struct DDS_HEADER
{
	int32_t size;
	int32_t flags;
	int32_t height;
	int32_t width;
	int32_t pitch;
	int32_t depth;
	int32_t mipMaps;
	int32_t reserved1[11];
	DDS_PIXELFORMAT format;
	int32_t caps[4];
	int32_t reserved2;
};

class Texture
{
public:
	Texture();
	Texture( const Texture& ref );
	~Texture();

	Texture& operator=( const Texture& ref );

	bool load( const char* path );
	void upload();
	void unload();
	void bind( GLenum location ) const;

	GLuint getID() const;
	int getWidth() const;
	int getHeight() const;

private:
	GLuint id;
	GLbyte* pixels;
	int width;
	int height;
	GLenum format;
	int size;
};