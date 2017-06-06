#pragma once

#include "BaseIncludes.h"

class Camera
{
public:
	Camera();
	~Camera();

	void updatePosition( const glm::vec3& localMovement );
	void updateDirection( int deltaX, int deltaY );
	void updatePerspective( float width, float height );
	void updateOrthographic( float width, float height );

	const glm::mat4& getViewMatrix();
	const glm::mat4& getProjectionMatrix();

private: 
	glm::vec3 position;
	glm::vec3 direction;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	bool dirtyViewMatrix;
	float horizontalAngle;
	float verticalAngle;
};