#pragma once

#include "BaseIncludes.h"

#define FONT_START 32
#define FONT_END 127
#define FONT_RANGE (FONT_END-FONT_START)

struct FontInfo
{
	uint8_t height;
	uint8_t shadow_x;
	uint8_t shadow_y;
	uint8_t padding_x;
	uint8_t padding_y;
	uint16_t textureSize;
	uint8_t widths[FONT_RANGE];
	uint16_t xoffsets[FONT_RANGE];
	uint16_t yoffsets[FONT_RANGE];
};

class Font
{
public:
	Font();
	Font( const Font& ref );
	~Font();

	Font& operator=( const Font& ref );

	bool load( const char* path );

	float getHeight() const;
	float getTextureSize() const;
	float getWidth( int index ) const;
	glm::vec2 getSize( int index ) const;
	glm::vec2 getOffset( int index ) const;
	glm::vec2 getStartUV( int index ) const;
	glm::vec2 getEndUV( int index ) const;

	const FontInfo& getInfo() const;
	bool getValid() const;

private:
	FontInfo info;
	bool valid;
};