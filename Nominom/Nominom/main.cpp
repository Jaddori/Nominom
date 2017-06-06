#include "BaseIncludes.h"

int main( int argc, char* argv[] )
{
	SDL_Window* window = SDL_CreateWindow( "Nominom", 32, 32, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );
	if( window )
	{
		SDL_GLContext glContext = SDL_GL_CreateContext( window );
		if( glContext )
		{
			bool running = true;
			SDL_Event e;
			while( running )
			{
				SDL_PollEvent( &e );
				switch( e.type )
				{
				case SDL_QUIT:
					running = false;
					break;

				case SDL_KEYDOWN:
					if( e.key.keysym.sym == SDLK_ESCAPE )
					{
						running = false;
					}
					break;
				}

				glClearColor( 1.0f, 0.0f, 0.0f, 1.0f );
				glClear( GL_COLOR_BUFFER_BIT );

				SDL_GL_SwapWindow( window );
				SDL_Delay( 20 );
			}

			SDL_GL_DeleteContext( glContext );
		}

		SDL_DestroyWindow( window );
	}

	return 0;
}
