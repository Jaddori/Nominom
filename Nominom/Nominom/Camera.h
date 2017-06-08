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
	void finalize();

	void setPosition( const glm::vec3& position );
	void setDirection( const glm::vec3& direction );

	const glm::vec3& getPosition() const;
	const glm::vec3& getDirection() const;

	const glm::mat4& getViewMatrix();
	const glm::mat4& getProjectionMatrix();

	const glm::mat4& getFinalViewMatrix();
	const glm::mat4& getFinalProjectionMatrix();

private: 
	glm::vec3 position;
	glm::vec3 direction;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	glm::mat4 finalViewMatrix;
	glm::mat4 finalProjectionMatrix;

	bool dirtyViewMatrix;
	float horizontalAngle;
	float verticalAngle;
};