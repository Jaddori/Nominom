#pragma once

#include "BaseIncludes.h"
#include "Shader.h"
#include "ModelInstance.h"
#include "Camera.h"

enum
{
	TARGET_DIFFUSE = 0,
	TARGET_POSITION,
	TARGET_NORMAL,
	TARGET_DEPTH,
	MAX_TARGETS
};

class GBuffer
{
public:
	GBuffer();
	~GBuffer();

	bool load( int width, int height );
	void upload();

	void begin();
	void end();

	void beginGeometryPass( Camera* camera );
	void endGeometryPass();
	void updateGeometryWorldMatrices( const glm::mat4* worldMatrices, int count );
	//void renderGeometry( Camera* camera, Array<ModelInstance>& instances );
	void renderDirectionalLights( Camera* camera );
	void renderPointLights( Camera* camera );
	void renderSpotLights( Camera* camera );

	GLuint getFBO() const;

private:
	int width;
	int height;
	GLuint fbo;
	GLuint targets[MAX_TARGETS];

	Shader geometryPass;
	GLuint geometryProjectionMatrix;
	GLuint geometryViewMatrix;
	GLuint geometryWorldMatrices;
	GLuint geometryCameraPosition;

	Shader directionalLightPass;


	Shader pointLightPass;


	Shader spotLightPass;
};