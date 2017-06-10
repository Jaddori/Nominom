#include "Transform.h"

Transform::Transform()
	: scale( 1.0f )
{
}

Transform::~Transform()
{
}

void Transform::setPosition( const glm::vec3& p )
{
	position = p;
}

void Transform::setRotation( const glm::vec3& r )
{
	rotation = r;
}

void Transform::setScale( const glm::vec3& s )
{
	scale = s;
}

const glm::vec3& Transform::getPosition() const
{
	return position;
}

const glm::vec3& Transform::getRotation() const
{
	return rotation;
}

const glm::vec3& Transform::getScale() const
{
	return scale;
}