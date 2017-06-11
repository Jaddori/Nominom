#include "Transform.h"

Transform::Transform()
	: scale( 1.0f ), dirtyFlag( true )
{
}

Transform::~Transform()
{
}

void Transform::setPosition( const glm::vec3& p )
{
	position = p;
	dirtyFlag = true;
}

void Transform::setRotation( const glm::vec3& r )
{
	rotation = r;
	dirtyFlag = true;
}

void Transform::setScale( const glm::vec3& s )
{
	scale = s;
	dirtyFlag = true;
}

void Transform::setDirtyFlag( bool dirty )
{
	dirtyFlag = dirty;
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

bool Transform::getDirtyFlag() const
{
	return dirtyFlag;
}