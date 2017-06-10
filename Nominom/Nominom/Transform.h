#pragma once

#include "BaseIncludes.h"
#include "Component.h"

class Transform : public Component
{
public:
	Transform();
	virtual ~Transform();

	void setPosition( const glm::vec3& position );
	void setRotation( const glm::vec3& rotation );
	void setScale( const glm::vec3& scale );

	const glm::vec3& getPosition() const;
	const glm::vec3& getRotation() const;
	const glm::vec3& getScale() const;

private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};