#pragma once

#include "BaseIncludes.h"
#include "ModelInstance.h"
#include "TextInstance.h"
#include "Assets.h"
#include "Shader.h"
#include "Camera.h"
#include "GBuffer.h"
#include "DebugShapes.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	void load( Assets* assets );
	void upload();

	void queueInstances( Array<ModelInstance>* instances );
	void queueTextInstances( Array<TextInstance>* textInstances );
	void queueDirectionalLights( Array<DirectionalLight>* lights );
	void queuePointLights( Array<PointLight>* lights );
	void render( Assets* assets );
	void finalize();

	Camera* getPerspectiveCamera();
	Camera* getOrthographicCamera();
	GBuffer* getGBuffer();
	DebugShapes* getDebugShapes();

private:
	Array<ModelInstance>* instances;
	Array<TextInstance>* textInstances;
	Array<DirectionalLight>* directionalLights;
	Array<PointLight>* pointLights;

	Camera perspectiveCamera;
	Camera orthographicCamera;
	GBuffer gbuffer;
	DebugShapes debugShapes;
	
	Shader textShader;
	GLint textProjectionMatrix;
	GLint textDiffuseMap;
};