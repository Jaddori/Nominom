#include "TextInstance.h"

TextInstance::TextInstance()
	: font( nullptr ), text( nullptr ), dirtyFlag( true )
{
}

TextInstance::TextInstance( Font* f, const char* t, const glm::vec2& p, const glm::vec4& c )
	: font( f ), text( t ), position( p ), color( c ), dirtyFlag( true )
{
}

TextInstance::~TextInstance()
{
}

void TextInstance::upload()
{
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	glEnableVertexAttribArray( 0 );
	glEnableVertexAttribArray( 1 );
	glEnableVertexAttribArray( 2 );
	glEnableVertexAttribArray( 3 );
	glEnableVertexAttribArray( 4 );
	AGLOG( "TextInstance(upload) ");

	glGenBuffers( 1, &vbo );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );

	const int STRIDE = sizeof(GLfloat) * 12;
	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, STRIDE, 0 );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(GLfloat)*2) );
	glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(GLfloat)*4) );
	glVertexAttribPointer( 3, 2, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(GLfloat)*6) );
	glVertexAttribPointer( 4, 4, GL_FLOAT, GL_FALSE, STRIDE, (void*)(sizeof(GLfloat)*8) );

	glBindVertexArray( 0 );
	AGLOG( "TextInstance(upload)" );
}

void TextInstance::render()
{
	glBindVertexArray( vao );
	glDrawArrays( GL_POINTS, 0, characters.getSize() );
	glBindVertexArray( 0 );
	AGLOG( "TextInstance(upload)" );
}

void TextInstance::finalize()
{
	if( dirtyFlag )
	{
		glm::vec2 positionOffset;

		characters.clear();
		const char* cur = text;
		while( *cur )
		{
			char c = *cur;
			if( c >= FONT_START && c < FONT_END )
			{
				c -= FONT_START;

				TextInfo character;
				character.position = position + positionOffset;
				character.size = font->getSize( c );
				character.startUV = font->getStartUV( c );
				character.endUV = font->getEndUV( c );
				character.color = color;

				characters.add( character );
				positionOffset.x += character.size.x + 0.5f;

				cur++;
			}
			else if( c == '\n' )
			{
				positionOffset.y += font->getInfo().height;
				positionOffset.x = 0.0f;
			}
		}

		glBindVertexArray( vao );
		glBindBuffer( GL_ARRAY_BUFFER, vbo );
		glBufferData( GL_ARRAY_BUFFER, sizeof(TextInfo)*characters.getSize(), characters.getData(), GL_DYNAMIC_DRAW );
		glBindVertexArray( 0 );
		AGLOG( "TextInstance(finalize)" );

		dirtyFlag = false;
	}
}

void TextInstance::setFont( Font* f )
{
	font = f;
	dirtyFlag = true;
}

void TextInstance::setText( const char* t )
{
	text = t;
	dirtyFlag = true;
}

void TextInstance::setPosition( const glm::vec2& p )
{
	position = p;
	dirtyFlag = true;
}

void TextInstance::setColor( const glm::vec4& c )
{
	color = c;
	dirtyFlag = true;
}

Font* TextInstance::getFont() const
{
	return font;
}

const char* TextInstance::getText() const
{
	return text;
}

const glm::vec2& TextInstance::getPosition() const
{
	return position;
}

const glm::vec4& TextInstance::getColor() const
{
	return color;
}