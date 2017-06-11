#pragma once

#include "BaseIncludes.h"
#include "Shader.h"
#include "ModelInstance.h"
#include "Camera.h"
#include "Assets.h"

#define GBUFFER_SPHERE_MESH_PATH "./assets/meshes/sphere.mesh"

struct DirectionalLight
{
	glm::vec3 direction;
	glm::vec3 color;
	float intensity;
};

struct PointLight
{
	glm::vec3 position;
	glm::vec3 color;
	float intensity;
	float linear;
	float constant;
	float exponent;
};

struct Billboard
{
	glm::vec3 position;
	glm::vec2 size;
};

enum
{
	TARGET_DIFFUSE = 0,
	TARGET_POSITION,
	TARGET_NORMAL,
	TARGET_DEPTH,
	TARGET_LIGHT,
	TARGET_BILLBOARD,
	TARGET_FINAL,
	MAX_TARGETS
};

class GBuffer
{
public:
	GBuffer();
	~GBuffer();

	bool load( Assets* assets, int width, int height );
	void upload();

	void begin();
	void end();

	void beginGeometryPass( Camera* camera );
	void endGeometryPass();
	void updateGeometryWorldMatrices( const glm::mat4* worldMatrices, int count );
	void updateGeometryTextures( Texture* diffuseMap, Texture* normalMap, Texture* specularMap );
	void beginDirectionalLightPass( Camera* camera );
	void endDirectionalLightPass();
	//void renderDirectionalLight( const glm::vec3& direction, const glm::vec3& color, float intensity );
	void renderDirectionalLight( const DirectionalLight& light );

	void beginPointLightPass( Camera* camera );
	void endPointLightPass();
	//void renderPointLight( Camera* camera, const glm::vec3& position, float radius, const glm::vec3& color, float intensity );
	void renderPointLight( const PointLight& light );

	void beginBillboardPass( Camera* camera );
	void endBillboardPass();
	void renderBillboards( Array<Billboard>* billboards );

	void performFinalPass();

	void setDebug( bool debug );
	void toggleDebug();

	GLuint getFBO() const;

private:
	int width;
	int height;
	GLuint fbo;
	GLuint targets[MAX_TARGETS];
	GLuint depthBuffer;
	bool debug;
	Assets* assets;

	GLuint quadVAO;

	Shader geometryPass;
	GLint geometryProjectionMatrix;
	GLint geometryViewMatrix;
	GLint geometryWorldMatrices;
	GLint geometryDiffuseMap;
	GLint geometryNormalMap;
	GLint geometryPositionMap;
	GLint geometryDepthMap;
	GLint geometryFarPlane;
	GLint geometryNearPlane;

	Shader directionalLightPass;
	GLint directionalLightDirection;
	GLint directionalLightColor;
	GLint directionalLightIntensity;
	GLint directionalLightCameraPosition;
	GLint directionalLightSpecularPower;
	GLint directionalLightDiffuseTarget;
	GLint directionalLightNormalTarget;
	GLint directionalLightPositionTarget;
	GLint directionalLightDepthTarget;

	Shader pointLightPass;
	GLint pointLightProjectionMatrix;
	GLint pointLightViewMatrix;
	GLint pointLightWorldMatrix;
	GLint pointLightCameraPosition;
	GLint pointLightSpecularPower;
	GLint pointLightScreenSize;
	GLint pointLightPosition;
	GLint pointLightRadius;
	GLint pointLightColor;
	GLint pointLightIntensity;
	GLint pointLightLinear;
	GLint pointLightConstant;
	GLint pointLightExponent;
	GLint pointLightDiffuseTarget;
	GLint pointLightNormalTarget;
	GLint pointLightPositionTarget;
	int sphereMesh;

	Shader spotLightPass;

	Shader billboardPass;
	GLint billboardProjectionMatrix;
	GLint billboardViewMatrix;
	GLint billboardCameraPosition;
	GLint billboardDiffuseMap;
	GLint billboardNormalMap;
	GLint billboardSpecularMap;
	GLuint billboardVAO;
	GLuint billboardVBO;

	Shader finalPass;
	GLint finalLightTarget;
	GLint finalBillboardTarget;
};