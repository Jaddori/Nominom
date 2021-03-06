#include "BaseIncludes.h"
#include "Renderer.h"
#include "Shader.h"
#include "Camera.h"
#include "Input.h"
#include "DebugShapes.h"
#include "Actor.h"
#include "MeshRenderer.h"
#include "InstanceHandler.h"
#include "TextInstance.h"

#define FPS 60
#define TICKS (1000 / FPS)

struct ThreadData
{
	Renderer* renderer;
	Assets* assets;
	Input* input;
	InstanceHandler* instanceHandler;
	Array<Actor>* actors;
	Array<DirectionalLight>* directionalLights;
	Array<PointLight>* pointLights;
	DebugShapes* debugShapes;
	SDL_sem* updateLock;
	SDL_sem* renderLock;
	bool running;
};

int updateThread( void* args )
{
	ThreadData* data = (ThreadData*)args;

	Camera* perspectiveCamera = data->renderer->getPerspectiveCamera();

	DebugSphere sphere = { glm::vec3( 0.0f ), 1.5f, glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f ) };
	DebugLine line = { glm::vec3( 0.0f ), glm::vec3( 0.0f, 10.0f, 0.0f ), glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f ) };
	DebugAABB aabb = { glm::vec3( -3.0f ), glm::vec3( 3.0f ), glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f ) };
	DebugOBB obb = { glm::vec3( 0.0f ), glm::vec3(0.7f, 0.0f, 0.7f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(-0.7f, 0.0f, 0.7f), glm::vec3(3.0f), glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f ) };

	glm::vec3 dir = glm::normalize( glm::vec3( 1.0f, -1.0f, 1.0f ) );

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
					perspectiveCamera->updateDirection( dx, dy );
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
				data->renderer->getGBuffer()->toggleDebugMode();
			}

			if( glm::length( localMovement ) > 0.0f )
			{
				perspectiveCamera->updatePosition( localMovement );
			}

			// Add debug lines for all directional lights
			const int NUM_DIRECTIONAL_LIGHTS = data->directionalLights->getSize();
			for( int i=0; i<NUM_DIRECTIONAL_LIGHTS; i++ )
			{
				const DirectionalLight& light = data->directionalLights->at( i );

				DebugLine line = { light.direction*-10.0f, glm::vec3( 0.0f ), glm::vec4( light.color, light.intensity ) };

				data->debugShapes->addLine( line );
			}

			// Add debug spheres for all point lights
			const int NUM_POINT_LIGHTS = data->pointLights->getSize();
			for( int i=0; i<NUM_POINT_LIGHTS; i++ )
			{
				const PointLight& light = data->pointLights->at( i );

				DebugSphere sphere = { light.position, 1.0f, glm::vec4( light.color, light.intensity ) };

				data->debugShapes->addSphere( sphere );
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
	
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

	SDL_Window* window = SDL_CreateWindow( "Nominom", 32, 32, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );
	if( window )
	{
		SDL_GLContext glContext = SDL_GL_CreateContext( window );
		if( glContext )
		{
#if WIN32
			glewExperimental = GL_TRUE;
			if( glewInit() != GLEW_OK )
			{
				return -1;
			}
#endif
			GLenum glewError = glGetError();
			if( glewError == GL_INVALID_ENUM )
			{
				LOG( VERBOSITY_WARNING, "main", "GLEW flagged error 1280 after glewInit()." );
			}

			Renderer renderer;
			Camera* perspectiveCamera = renderer.getPerspectiveCamera();
			Assets assets;
			Input input;
			InstanceHandler instanceHandler;
			Array<Actor> actors;
			Array<TextInstance> textInstances;
			Array<DirectionalLight> directionalLights;
			Array<PointLight> pointLights;
			//DebugShapes debugShapes;
			DebugShapes* debugShapes = renderer.getDebugShapes();

			int mesh = assets.loadMesh( "./assets/meshes/test.mesh" );
			int diffuseMap = assets.loadTexture( "./assets/textures/crate_diffuse.dds" );
			int normalMap = assets.loadTexture( "./assets/textures/crate_normal.dds" );
			int specularMap = assets.loadTexture( "./assets/textures/crate_specular.dds" );

			int faceDiffuseMap = assets.loadTexture( "./assets/textures/face_diffuse.dds" );
			int faceNormalMap = assets.loadTexture( "./assets/textures/face_normal.dds" );
			int faceSpecularMap = assets.loadTexture( "./assets/textures/face_specular.dds" );

			int fontTexture = assets.loadTexture( "./assets/fonts/verdana_24.dds" );
			Font* font = assets.loadFont( "./assets/fonts/verdana_24.font" );
			assert( font );

			const char* fontText = "Testing...";
			textInstances.add( TextInstance( font, fontText, glm::vec2( 32.0f, 32.0f ), glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f ) ) );
			textInstances[0].upload();

			renderer.queueInstances( instanceHandler.getInstances() );
			renderer.queueTextInstances( &textInstances );
			renderer.queueDirectionalLights( &directionalLights );
			renderer.queuePointLights( &pointLights );
			renderer.load( &assets );
			renderer.upload();

			Actor actor;
			Transform transform;
			MeshRenderer meshRenderer;
			meshRenderer.load( &assets, &instanceHandler );

			actor.addComponent( &transform );
			actor.addComponent( &meshRenderer );

			Actor actor2;
			Transform transform2;
			transform2.setPosition( glm::vec3( -4.0f, 0.0f, 0.0f ) );
			MeshRenderer meshRenderer2;
			meshRenderer2.load( &assets, &instanceHandler );

			actor2.addComponent( &transform2 );
			actor2.addComponent( &meshRenderer2 );

			Actor actor3;
			Transform transform3;
			transform3.setPosition( glm::vec3( 10.0f, 0.0f, 0.0f ) );
			MeshRenderer meshRenderer3;
			meshRenderer3.load( &assets, &instanceHandler );

			actor3.addComponent( &transform3 );
			actor3.addComponent( &meshRenderer3 );

			Actor actor4;
			Transform transform4;
			transform4.setPosition( glm::vec3( -2.0f, -1.0f, 2.0f ) );
			MeshRenderer meshRenderer4;
			meshRenderer4.load( &assets, &instanceHandler );

			actor4.addComponent( &transform4 );
			actor4.addComponent( &meshRenderer4 );

			debugShapes->load();
			debugShapes->upload();

			assets.upload();

			perspectiveCamera->setPosition( glm::vec3( 0, 0, -4.0f ) );

			DirectionalLight directionalLight =
			{
				glm::normalize( glm::vec3( 1.0f, -1.0f, 1.0f ) ),
				glm::vec3( 1.0f, 0.0f, 0.0f ),
				0.8f
			};

			DirectionalLight directionalLight2 =
			{
				glm::normalize( glm::vec3( -1.0f, -1.0f, 1.0f ) ),
				glm::vec3( 0.0f, 0.0f, 1.0f ),
				0.8f
			};

			directionalLights.add( directionalLight );
			directionalLights.add( directionalLight2 );

			PointLight pointLight =
			{
				glm::vec3( 0.0f, 1.0f, 2.1f ),
				glm::vec3( 0.0f, 1.0f, 0.0f ),
				2.0f,
				1.0f,
				0.0f,
				1.0f
			};
			pointLights.add( pointLight );

			ThreadData data =
			{
				&renderer,
				&assets,
				&input,
				&instanceHandler,
				&actors,
				&directionalLights,
				&pointLights,
				debugShapes,
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

				meshRenderer.finalize();
				meshRenderer2.finalize();
				meshRenderer3.finalize();
				meshRenderer4.finalize();
				renderer.finalize();
				//debugShapes.finalize();

				SDL_SemPost( data.updateLock );
				// END OF CRITICAL SECTION

				renderer.render( &assets );
				//debugShapes.render( perspectiveCamera );

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
