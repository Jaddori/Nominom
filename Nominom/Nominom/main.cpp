#include "BaseIncludes.h"
#include "Renderer.h"
#include "Shader.h"
#include "Camera.h"
#include "Input.h"

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
			Input input;
			Array<ModelInstance> instances;

			assert( shader.load( "./assets/shaders/basic.vs", nullptr, "./assets/shaders/basic.fs" ) );
			shader.upload();

			GLint projectionMatrixLocation = shader.getUniform( "projectionMatrix" );
			GLint viewMatrixLocation = shader.getUniform( "viewMatrix" );
			GLint worldMatrixLocation = shader.getUniform( "worldMatrices" );

			camera.updatePerspective( 640.0f, 480.0f );
			camera.setPosition( glm::vec3( 0, 0, -5.0f ) );

			int mesh = assets.loadMesh( "./assets/meshes/cube.mesh" );
			int texture = assets.loadTexture( "./assets/textures/grass.dds" );

			instances.add( ModelInstance( mesh, texture ) );
			int firstIndex = instances[0].add();
			int secondIndex = instances[0].add();

			instances[0].setWorldMatrix( secondIndex, glm::translate( *instances[0].getWorldMatrix( secondIndex ), glm::vec3( -4.0f, 0.0f, 0.0f ) ) );

			renderer.queue( &instances );

			assets.upload();

			glEnable( GL_DEPTH_TEST );
			glEnable( GL_CULL_FACE );

			bool running = true;
			SDL_Event e;
			while( running )
			{
				SDL_PollEvent( &e );
				input.update( &e );

				if( e.type == SDL_QUIT || input.keyReleased( SDL_SCANCODE_ESCAPE ) )
				{
					running = false;
				}

				if( input.buttonDown( SDL_BUTTON_LEFT ) )
				{
					if( input.getMouseDeltaX() || input.getMouseDeltaY() )
					{
						camera.updateDirection( input.getMouseDeltaX(), input.getMouseDeltaY() );
					}
				}

				glm::vec3 localMovement;
				if( input.keyDown( SDL_SCANCODE_W ) )
				{
					localMovement.z += 1.0f;
				}
				if( input.keyDown( SDL_SCANCODE_S ) )
				{
					localMovement.z -= 1.0f;
				}
				if( input.keyDown( SDL_SCANCODE_D ) )
				{
					localMovement.x += 1.0f;
				}
				if( input.keyDown( SDL_SCANCODE_A ) )
				{
					localMovement.x -= 1.0f;
				}
				if( input.keyDown( SDL_SCANCODE_SPACE ) )
				{
					localMovement.y += 1.0f;
				}
				if( input.keyDown( SDL_SCANCODE_LCTRL ) )
				{
					localMovement.y -= 1.0f;
				}

				if( glm::length( localMovement ) > 0.0f )
				{
					camera.updatePosition( localMovement );
				}

				glClearColor( 0.0f, 0.0f, 1.0f, 1.0f );
				glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

				instances[0].finalize();

				shader.bind();
				shader.setMat4( projectionMatrixLocation, &camera.getProjectionMatrix(), 1 );
				shader.setMat4( viewMatrixLocation, &camera.getViewMatrix(), 1 );
				shader.setMat4( worldMatrixLocation, instances[0].getFinalMatrices(), instances[0].getInstances() );

				renderer.render( &assets );

				SDL_GL_SwapWindow( window );
				SDL_Delay( 20 );
			}

			SDL_GL_DeleteContext( glContext );
		}

		SDL_DestroyWindow( window );
	}

	return 0;
}
