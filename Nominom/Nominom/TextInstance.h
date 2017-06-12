#pragma once

#include "BaseIncludes.h"
#include "Font.h"

struct TextInfo
{
	glm::vec2 position;
	glm::vec2 size;
	glm::vec2 startUV;
	glm::vec2 endUV;
	glm::vec4 color;
};

class TextInstance
{
public:
	TextInstance();
	TextInstance( Font* font, const char* text, const glm::vec2& position, const glm::vec4& color );
	virtual ~TextInstance();

	void upload();
	void render();
	void finalize();

	void setFont( Font* font );
	void setText( const char* text );
	void setPosition( const glm::vec2& position );
	void setColor( const glm::vec4& color );

	Font* getFont() const;
	const char* getText() const;
	const glm::vec2& getPosition() const;
	const glm::vec4& getColor() const;

private:
	Font* font;
	const char* text;
	glm::vec2 position;
	glm::vec4 color;

	Array<TextInfo> characters;
	bool dirtyFlag;

	GLuint vao;
	GLuint vbo;
};