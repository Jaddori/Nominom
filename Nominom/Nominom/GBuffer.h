#pragma once

#include "BaseIncludes.h"
#include "Shader.h"
#include "ModelInstance.h"
#include "Camera.h"
#include "Assets.h"

#define GBUFFER_SPHERE_MESH_PATH "./assets/meshes/sphere.mesh"
#define GBUFFER_SHADOW_MAP_RESOLUTION 10.0f

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
	TARGET_SHADOW,
	TARGET_ALPHA = TARGET_SHADOW,
	TARGET_LIGHT,
	TARGET_BILLBOARD,
	TARGET_FINAL,
	MAX_TARGETS
};

enum
{
	DEBUG_NONE = 0,
	DEBUG_GEOMETRY,
	DEBUG_FINAL,
	MAX_DEBUG_MODES
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

	void beginDirectionalShadowPass( Camera* camera, const DirectionalLight& light );
	void endDirectionalShadowPass();
	void updateDirectionalShadowWorldMatrices( const glm::mat4* worldMatrices, int count );

	void beginDirectionalLightPass( int target, Camera* camera );
	void endDirectionalLightPass();
	void renderDirectionalLight( Camera* camera, const DirectionalLight& light );

	void beginPointLightPass( int target, Camera* camera );
	void endPointLightPass();
	void renderPointLight( const PointLight& light );

	void beginBillboardPass( Camera* camera );
	void endBillboardPass();
	void renderBillboards( Array<Billboard>* billboards );

	void performFinalPass();

	void setDebugMode( int mode );
	void toggleDebugMode();

	GLuint getFBO() const;
	GLuint getTarget( int index ) const;

private:
	int width;
	int height;
	GLuint fbo;
	GLuint targets[MAX_TARGETS];
	GLuint depthBuffer;
	int debugMode;
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

	Shader directionalLightPass;
	GLint directionalLightDirection;
	GLint directionalLightColor;
	GLint directionalLightIntensity;
	GLint directionalLightCameraPosition;
	GLint directionalLightSpecularPower;
	GLint directionalLightTransformation;
	GLint directionalLightDiffuseTarget;
	GLint directionalLightNormalTarget;
	GLint directionalLightPositionTarget;
	GLint directionalLightDepthTarget;
	GLint directionalLightShadowTarget;

	Shader directionalShadowPass;
	GLint directionalShadowProjectionMatrix;
	GLint directionalShadowViewMatrix;
	GLint directionalShadowWorldMatrices;

	Shader pointLightPass;
	GLint pointLightProjectionMatrix;
	GLint pointLightViewMatrix;
	GLint pointLightWorldMatrix;
	GLint pointLightCameraPosition;
	GLint pointLightScreenSize;
	GLint pointLightSpecularPower;
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

	Shader pointShadowPass;
	GLint pointShadowProjectionMatrix;
	GLint pointShadowViewMatrix;
	GLint pointShadowWorldMatrices;

	Shader spotLightPass;

	Shader billboardPass;
	GLint billboardProjectionMatrix;
	GLint billboardViewMatrix;
	GLint billboardScreenSize;
	GLint billboardDiffuseMap;
	GLint billboardNormalMap;
	GLint billboardSpecularMap;
	GLint billboardDepthTarget;
	GLuint billboardVAO;
	GLuint billboardVBO;

	Shader finalPass;
	GLint finalLightTarget;
	GLint finalBillboardTarget;
	GLint finalBillboardAlphaTarget;
};