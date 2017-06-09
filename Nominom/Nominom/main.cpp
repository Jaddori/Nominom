#include "BaseIncludes.h"
#include "Renderer.h"
#include "Shader.h"
#include "Camera.h"
#include "Input.h"
#include "DebugShapes.h"

#define FPS 60
#define TICKS (1000 / FPS)

struct ThreadData
{
	Renderer* renderer;
	Assets* assets;
	Input* input;
	Array<ModelInstance>* instances;
	DebugShapes* debugShapes;
	SDL_sem* updateLock;
	SDL_sem* renderLock;
	bool running;
};

int updateThread( void* args )
{
	ThreadData* data = (ThreadData*)args;

	Camera* camera = data->renderer->getCamera();

	DebugSphere sphere = { glm::vec3( 0.0f ), 1.5f, glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f ) };
	DebugLine line = { glm::vec3( 0.0f ), glm::vec3( 0.0f, 10.0f, 0.0f ), glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f ) };
	DebugAABB aabb = { glm::vec3( -3.0f ), glm::vec3( 3.0f ), glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f ) };
	DebugOBB obb = { glm::vec3( 0.0f ), glm::vec3(0.7f, 0.0f, 0.7f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(-0.7f, 0.0f, 0.7f), glm::vec3(3.0f), glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f ) };

	while( data->running )
	{
		if( SDL_SemWaitTimeout( data->updateLock, 100 ) == 0 )
		{
			if( data->input->buttonDown( SDL_BUTTON_LEFT ) )
			{
				int dx = data->input->getMouseDeltaX();
				int dy = data->input->getMouseDeltaY();
				if( dx || dy )
				{
					camera->updateDirection( dx, dy );
				}
			}

			glm::vec3 localMovement;
			if( data->input->keyDown( SDL_SCANCODE_W ) )
			{
				localMovement.z += 1.0f;
			}
			if( data->input->keyDown( SDL_SCANCODE_S ) )
			{
				localMovement.z -= 1.0f;
			}
			if( data->input->keyDown( SDL_SCANCODE_D ) )
			{
				localMovement.x += 1.0f;
			}
			if( data->input->keyDown( SDL_SCANCODE_A ) )
			{
				localMovement.x -= 1.0f;
			}
			if( data->input->keyDown( SDL_SCANCODE_SPACE ) )
			{
				localMovement.y += 1.0f;
			}
			if( data->input->keyDown( SDL_SCANCODE_LCTRL ) )
			{
				localMovement.y -= 1.0f;
			}
			if( data->input->keyReleased( SDL_SCANCODE_G ) )
			{
				data->renderer->getGBuffer()->toggleDebug();
			}

			if( glm::length( localMovement ) > 0.0f )
			{
				camera->updatePosition( localMovement );
			}

			data->instances->at( 0 ).setDirty( true );

			/*data->debugShapes->addSphere( sphere );
			data->debugShapes->addLine( line );
			data->debugShapes->addAABB( aabb );
			data->debugShapes->addOBB( obb );*/

			Mesh* mesh = data->assets->getMesh( 0 );
			int count;
			const Vertex* vertices = mesh->getVertices( &count );
			for( int i=0; i<count; i++ )
			{
				DebugLine normalLine = { vertices[i].position, vertices[i].position+vertices[i].normal*0.1f, glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f ) };

				DebugLine tangentLine = { vertices[i].position, vertices[i].position+vertices[i].tangent*0.1f, glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f ) };

				DebugLine bitangentLine = { vertices[i].position, vertices[i].position+vertices[i].bitangent*0.1f, glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f ) };

				data->debugShapes->addLine( normalLine );
				data->debugShapes->addLine( tangentLine );
				data->debugShapes->addLine( bitangentLine );
			}

			SDL_SemPost( data->renderLock );
		}
	}

	return 0;
}

int main( int argc, char* argv[] )
{
	Logger::instance().start();
	Logger::instance().setVerbosity( VERBOSITY_WARNING );

	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
	{
		return -1;
	}

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
			Camera* camera = renderer.getCamera();
			Assets assets;
			Input input;
			Array<ModelInstance> instances;
			DebugShapes debugShapes;

			int mesh = assets.loadMesh( "./assets/meshes/test.mesh" );
			int diffuseMap = assets.loadTexture( "./assets/textures/crate_diffuse.dds" );
			int normalMap = assets.loadTexture( "./assets/textures/crate_normal.dds" );
			//int normalMap = assets.loadTexture( "./assets/textures/default_normal.dds" );
			int specularMap = assets.loadTexture( "./assets/textures/crate_specular.dds" );

			instances.add( ModelInstance( mesh, diffuseMap, normalMap, specularMap ) );
			int firstIndex = instances[0].add();
			int secondIndex = instances[0].add();

			instances[0].setWorldMatrix( secondIndex, glm::translate( *instances[0].getWorldMatrix( secondIndex ), glm::vec3( -4.0f, 0.0f, 0.0f ) ) );

			renderer.queue( &instances );
			renderer.load();
			renderer.upload();

			debugShapes.load();
			debugShapes.upload();

			assets.upload();

			camera->setPosition( glm::vec3( 0, 0, -4.0f ) );

			ThreadData data =
			{
				&renderer,
				&assets,
				&input,
				&instances,
				&debugShapes,
				SDL_CreateSemaphore(1),
				SDL_CreateSemaphore(0),
				true
			};

			SDL_Thread* thread = SDL_CreateThread( updateThread, "UpdateThread", &data );

			while( data.running )
			{
				// CRITICAL SECTION
				SDL_SemWait( data.renderLock );

				int startTime = SDL_GetTicks();
				if( !input.update() || input.keyReleased( SDL_SCANCODE_ESCAPE ) )
				{
					data.running = false;
				}

				renderer.finalize();
				debugShapes.finalize();

				/*for( int i=0; i<10; i++ )
				{
					sphere.position = glm::vec3( i, 0.0f, 0.0f );
					debugShapes.addSphere( sphere );
				}*/

				SDL_SemPost( data.updateLock );
				// END OF CRITICAL SECTION

				renderer.render( &assets );
				
				debugShapes.render( camera );

				SDL_GL_SwapWindow( window );
				int timeDif = SDL_GetTicks() - startTime;

				if( timeDif < TICKS )
				{
					SDL_Delay( TICKS - timeDif );
				}
			}

			SDL_WaitThread( thread, NULL );

			SDL_GL_DeleteContext( glContext );
		}

		SDL_DestroyWindow( window );
	}

	Logger::instance().stop();

	return 0;
}
