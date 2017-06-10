#pragma once

#include "BaseIncludes.h"
#include "ModelInstance.h"
#include "Assets.h"
#include "Shader.h"
#include "Camera.h"
#include "GBuffer.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	void load( Assets* assets );
	void upload();

	void queueInstances( Array<ModelInstance>* instances );
	void queueDirectionalLights( Array<DirectionalLight>* lights );
	void queuePointLights( Array<PointLight>* lights );
	void render( Assets* assets );
	void finalize();

	Camera* getCamera();
	GBuffer* getGBuffer();

private:
	Array<ModelInstance>* instances;
	Array<DirectionalLight>* directionalLights;
	Array<PointLight>* pointLights;

	Camera camera;
	GBuffer gbuffer;
	//Shader basicShader;
	GLuint worldMatricesLocation;
	GLuint viewMatrixLocation;
	GLuint projectionMatrixLocation;
};