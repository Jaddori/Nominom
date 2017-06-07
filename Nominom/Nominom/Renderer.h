#pragma once

#include "BaseIncludes.h"
#include "ModelInstance.h"
#include "Assets.h"
#include "Shader.h"
#include "Camera.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	void load();
	void upload();

	void queue( Array<ModelInstance>* instances );
	void render( Assets* assets );

	Camera* getCamera();

private:
	Array<ModelInstance>* instances;

	Camera camera;
	Shader basicShader;
	GLuint worldMatricesLocation;
	GLuint viewMatrixLocation;
	GLuint projectionMatrixLocation;
};