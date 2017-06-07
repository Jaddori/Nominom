#include "Input.h"

Input::Input()
{
	memset( keys, 0, sizeof(bool)*INPUT_MAX_KEYS );
	memset( prevKeys, 0, sizeof(bool)*INPUT_MAX_KEYS );
	memset( buttons, 0, sizeof(bool)*INPUT_MAX_BUTTONS );
	memset( prevButtons, 0, sizeof(bool)*INPUT_MAX_BUTTONS );

	mouseX = mouseY = -1;
	mouseDeltaX = mouseDeltaY = 0;
}

Input::~Input()
{
}

bool Input::update()
{
	bool result = true;

	memcpy( prevKeys, keys, sizeof(bool)*INPUT_MAX_KEYS );
	memcpy( prevButtons, buttons, sizeof(bool)*INPUT_MAX_BUTTONS );

	int prevMouseX = mouseX;
	int prevMouseY = mouseY;

	SDL_Event e;
	while( SDL_PollEvent( &e ) )
	{
		switch( e.type )
		{
		case SDL_KEYDOWN:
			keys[e.key.keysym.scancode] = true;
			break;

		case SDL_KEYUP:
			keys[e.key.keysym.scancode] = false;
			break;

		case SDL_MOUSEBUTTONDOWN:
			buttons[e.button.button] = true;
			break;

		case SDL_MOUSEBUTTONUP:
			buttons[e.button.button] = false;
			break;

		case SDL_QUIT:
			result = false;
			break;
		}
	}

	SDL_GetMouseState( &mouseX, &mouseY );

	if( prevMouseX >= 0 && prevMouseY >= 0 )
	{
		mouseDeltaX = prevMouseX - mouseX;
		mouseDeltaY = prevMouseY - mouseY;
	}

	return result;
}

bool Input::keyDown( int32_t key )
{
	assert( key < INPUT_MAX_KEYS );
	return keys[key];
}

bool Input::keyUp( int32_t key )
{
	assert( key < INPUT_MAX_KEYS );
	return !keys[key];
}

bool Input::keyPressed( int32_t key )
{
	assert( key < INPUT_MAX_KEYS );
	return ( !prevKeys[key] && keys[key] );
}

bool Input::keyReleased( int32_t key )
{
	assert( key < INPUT_MAX_KEYS );
	return ( !keys[key] && prevKeys[key] );
}

bool Input::buttonDown( uint8_t button )
{
	assert( button < INPUT_MAX_BUTTONS );
	return buttons[button];
}

bool Input::buttonUp( uint8_t button )
{
	assert( button < INPUT_MAX_BUTTONS );
	return !buttons[button];
}

bool Input::buttonPressed( uint8_t button )
{
	assert( button < INPUT_MAX_BUTTONS );
	return ( !prevButtons[button] && buttons[button] );
}

bool Input::buttonReleased( uint8_t button )
{
	assert( button < INPUT_MAX_BUTTONS );
	return ( !buttons[button] && prevButtons[button] );
}

int Input::getMouseX() const
{
	return mouseX;
}

int Input::getMouseY() const
{
	return mouseY;
}

int Input::getMouseDeltaX() const
{
	return mouseDeltaX;
}

int Input::getMouseDeltaY() const
{
	return mouseDeltaY;
}