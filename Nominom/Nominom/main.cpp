#include "BaseIncludes.h"
#include "Renderer.h"
#include "Shader.h"
#include "Camera.h"

int main( int argc, char* argv[] )
{
	SDL_Window* window = SDL_CreateWindow( "Nominom", 32, 32, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );
	if( window )
	{
		SDL_GLContext glContext = SDL_GL_CreateContext( window );
		if( glContext )
		{
			glewExperimental = GL_TRUE;
			if( glewInit() != GLEW_OK )
			{
				return -1;
			}

			Renderer renderer;
			Camera camera;
			Shader shader;
			Assets assets;

			assert( shader.load( "./assets/shaders/basic.vs", nullptr, "./assets/shaders/basic.fs" ) );
			shader.upload();

			GLint projectionMatrixLocation = shader.getUniform( "projectionMatrix" );
			GLint viewMatrixLocation = shader.getUniform( "viewMatrix" );
			GLint worldMatrixLocation = shader.getUniform( "worldMatrix" );

			camera.updatePerspective( 640.0f, 480.0f );
			camera.setPosition( glm::vec3( 0, 0, -5.0f ) );

			int mesh = assets.loadMesh( "./assets/meshes/cube.mesh" );
			int texture = assets.loadTexture( "./assets/textures/grass.dds" );

			assets.upload();

			glEnable( GL_DEPTH_TEST );
			glEnable( GL_CULL_FACE );

			int counter = 0;
			int prevMouseX = -1, prevMouseY = -1;

			bool lmb = false;
			bool w = false, a = false, s = false, d = false, space = false, ctrl = false;
			bool running = true;
			SDL_Event e;
			while( running )
			{
				int mouseX = 0, mouseY = 0;

				SDL_PollEvent( &e );
				switch( e.type )
				{
				case SDL_QUIT:
					running = false;
					break;

				case SDL_KEYDOWN:
					switch( e.key.keysym.sym )
					{
					case SDLK_ESCAPE:
						running = false;
						break;

					case SDLK_w:
						w = true;
						break;

					case SDLK_a:
						a = true;
						break;

					case SDLK_s:
						s = true;
						break;

					case SDLK_d:
						d = true;
						break;

					case SDLK_SPACE:
						space = true;
						break;

					case SDLK_LCTRL:
						ctrl = true;
						break;
					}
					break;

				case SDL_KEYUP:
					switch( e.key.keysym.sym )
					{
					case SDLK_w:
						w = false;
						break;

					case SDLK_a:
						a = false;
						break;

					case SDLK_s:
						s = false;
						break;

					case SDLK_d:
						d = false;
						break;

					case SDLK_SPACE:
						space = false;
						break;

					case SDLK_LCTRL:
						ctrl = false;
						break;
					}
					break;

				case SDL_MOUSEBUTTONDOWN:
					if( e.button.button == SDL_BUTTON_LEFT )
					{
						lmb = true;
					}
					break;

				case SDL_MOUSEBUTTONUP:
					if( e.button.button == SDL_BUTTON_LEFT )
					{
						lmb = false;
					}
					break;
				}

				SDL_GetMouseState( &mouseX, &mouseY );
				if( prevMouseX < 0 )
				{
					prevMouseX = mouseX;
					prevMouseY = mouseY;
				}
				else
				{
					int dx = prevMouseX - mouseX;
					int dy = prevMouseY - mouseY;

					prevMouseX = mouseX;
					prevMouseY = mouseY;

					mouseX = dx;
					mouseY = dy;
				}

				if( lmb && ( mouseX || mouseY ) )
				{
					camera.updateDirection( mouseX, mouseY );
				}

				glm::vec3 localMovement;
				if( w )
				{
					localMovement.z += 1.0f;
				}
				if( s )
				{
					localMovement.z -= 1.0f;
				}
				if( d )
				{
					localMovement.x += 1.0f;
				}
				if( a )
				{
					localMovement.x -= 1.0f;
				}
				if( space )
				{
					localMovement.y += 1.0f;
				}
				if( ctrl )
				{
					localMovement.y -= 1.0f;
				}

				if( glm::length( localMovement ) > 0.0f )
				{
					camera.updatePosition( localMovement );
				}

				glClearColor( 0.0f, 0.0f, 1.0f, 1.0f );
				glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

				shader.bind();
				shader.setMat4( projectionMatrixLocation, &camera.getProjectionMatrix(), 1 );
				shader.setMat4( viewMatrixLocation, &camera.getViewMatrix(), 1 );
				shader.setMat4( worldMatrixLocation, &glm::mat4(), 1 );

				Texture* textureptr = assets.getTexture( texture );
				textureptr->bind( GL_TEXTURE0 );

				Mesh* meshptr = assets.getMesh( mesh );
				meshptr->render( 1 );

				SDL_GL_SwapWindow( window );
				SDL_Delay( 20 );
			}

			SDL_GL_DeleteContext( glContext );
		}

		SDL_DestroyWindow( window );
	}

	return 0;
}
