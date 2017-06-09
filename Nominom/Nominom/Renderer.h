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

	void load();
	void upload();

	void queue( Array<ModelInstance>* instances );
	void render( Assets* assets );
	void finalize();

	Camera* getCamera();
	GBuffer* getGBuffer();

private:
	Array<ModelInstance>* instances;

	Camera camera;
	GBuffer gbuffer;
	//Shader basicShader;
	GLuint worldMatricesLocation;
	GLuint viewMatrixLocation;
	GLuint projectionMatrixLocation;
};