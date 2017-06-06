#pragma once

#include "BaseIncludes.h"

#define INPUT_MAX_KEYS 284 // https://wiki.libsdl.org/SDLScancodeLookup
#define INPUT_MAX_BUTTONS 5 // https://wiki.libsdl.org/SDL_MouseButtonEvent

class Input
{
public:
	Input();
	~Input();

	void update( SDL_Event* e );

	bool keyDown( int32_t key );
	bool keyUp( int32_t key );
	bool keyPressed( int32_t key );
	bool keyReleased( int32_t key );

	bool buttonDown( uint8_t key );
	bool buttonUp( uint8_t key );
	bool buttonPressed( uint8_t key );
	bool buttonReleased( uint8_t key );

	int getMouseX() const;
	int getMouseY() const;
	int getMouseDeltaX() const;
	int getMouseDeltaY() const;

private:
	bool keys[INPUT_MAX_KEYS];
	bool prevKeys[INPUT_MAX_KEYS];
	bool buttons[INPUT_MAX_BUTTONS];
	bool prevButtons[INPUT_MAX_BUTTONS];

	int mouseX, mouseY;
	int mouseDeltaX, mouseDeltaY;
};