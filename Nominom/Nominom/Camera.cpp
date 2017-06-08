#include "Camera.h"

Camera::Camera()
	: position( 0.0f ), direction( 0.0f, 0.0f, 1.0f ), dirtyViewMatrix( true ),
	horizontalAngle( 0.0f ), verticalAngle( 0.0f )
{
}

Camera::~Camera()
{
}

void Camera::updatePosition( const glm::vec3& localMovement )
{
	const float EPSILON = glm::epsilon<float>();
	const float PI = glm::pi<float>();

	// move backwards and forwards
	if( fabs( localMovement.z ) > EPSILON )
	{
		glm::vec3 forward = glm::normalize( direction );
		position += forward * localMovement.z;
	}

	// move left and right
	if( fabs( localMovement.x ) > EPSILON )
	{
		glm::vec3 right( sin( horizontalAngle - PI*0.5f ),
							0.0f,
							cos( horizontalAngle - PI*0.5f ) );
		position += right * localMovement.x;
	}

	// move up and down
	if( fabs( localMovement.y ) > EPSILON )
	{
		glm::vec3 up( 0.0f, 1.0f, 0.0f );
		position += up * localMovement.y;
	}

	dirtyViewMatrix = true;
}

void Camera::updateDirection( int deltaX, int deltaY )
{
	const float PI = glm::pi<float>();

	direction = glm::normalize( direction );

	horizontalAngle += (float)deltaX * 0.01f;
	verticalAngle += (float)deltaY * 0.01f;

	// clamp angles
	if( horizontalAngle > 2*PI )
	{
		horizontalAngle -= 2*PI;
	}
	else if( horizontalAngle < -2*PI )
	{
		horizontalAngle += 2*PI;
	}

	if( fabs( verticalAngle ) > PI*0.5f )
	{
		verticalAngle = PI*0.5f;
	}

	// calculate new direction
	direction = glm::vec3(
		cos( verticalAngle ) * sin( horizontalAngle ),
		sin( verticalAngle ),
		cos( verticalAngle ) * cos( horizontalAngle )
	);

	dirtyViewMatrix = true;
}

void Camera::updatePerspective( float width, float height )
{
	projectionMatrix = glm::perspectiveFov( 45.0f, width, height, 0.1f, 100.0f );
}

void Camera::updateOrthographic( float width, float height )
{
	projectionMatrix = glm::ortho( 0.0f, width, height, 0.0f );
}

void Camera::finalize()
{
	finalProjectionMatrix = getProjectionMatrix();
	finalViewMatrix = getViewMatrix();
}

void Camera::setPosition( const glm::vec3& p )
{
	position = p;
	dirtyViewMatrix = true;
}

void Camera::setDirection( const glm::vec3& d )
{
	direction = d;
	dirtyViewMatrix = true;
}

const glm::mat4& Camera::getViewMatrix()
{
	if( dirtyViewMatrix )
	{
		viewMatrix = glm::lookAt( position, position+direction, glm::vec3(0.0f, 1.0f, 0.0f) );
		dirtyViewMatrix = false;
	}

	return viewMatrix;
}

const glm::mat4& Camera::getProjectionMatrix()
{
	return projectionMatrix;
}

const glm::mat4& Camera::getFinalViewMatrix()
{
	return finalViewMatrix;
}

const glm::mat4& Camera::getFinalProjectionMatrix()
{
	return finalProjectionMatrix;
}