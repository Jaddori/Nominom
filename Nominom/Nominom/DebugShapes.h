#pragma once

#include "BaseIncludes.h"
#include "DebugSphere.h"
#include "DebugAABB.h"
#include "DebugOBB.h"
#include "DebugLine.h"
#include "Shader.h"
#include "Logger.h"
#include "Camera.h"

class DebugShapes
{
public:
	DebugShapes();
	~DebugShapes();

	bool load();
	void upload();

	void bindDepthTarget( GLuint target );
	void render( Camera* camera );
	void finalize();

	void addSphere( const DebugSphere& sphere );
	void addAABB( const DebugAABB& aabb );
	void addOBB( const DebugOBB& obb );
	void addLine( const DebugLine& line );

private:
	Array<DebugSphere> spheres;
	Array<DebugAABB> aabbs;
	Array<DebugOBB> obbs;
	Array<DebugLine> lines;

	Array<DebugSphere> finalSpheres;
	Array<DebugAABB> finalAABBs;
	Array<DebugOBB> finalOBBs;
	Array<DebugLine> finalLines;

	GLuint depthTarget;

	// sphere
	Shader sphereShader;
	GLint sphereProjectionMatrix;
	GLint sphereViewMatrix;
	GLint sphereDepthTarget;
	GLint sphereScreenSize;
	GLuint sphereVAO, sphereVBO;

	// aabb
	Shader aabbShader;
	GLint aabbProjectionMatrix;
	GLint aabbViewMatrix;
	GLint aabbDepthTarget;
	GLint aabbScreenSize;
	GLuint aabbVAO, aabbVBO;

	// obb
	Shader obbShader;
	GLint obbProjectionMatrix;
	GLint obbViewMatrix;
	GLint obbDepthTarget;
	GLint obbScreenSize;
	GLuint obbVAO, obbVBO;

	// line
	Shader lineShader;
	GLint lineProjectionMatrix;
	GLint lineViewMatrix;
	GLint lineDepthTarget;
	GLint lineScreenSize;
	GLuint lineVAO, lineVBO;
};