#pragma once

#include "BaseIncludes.h"

#define ACTOR_TAG_MAX_LEN 32

class Component;
class Actor
{
public:
	Actor();
	virtual ~Actor();

	void addComponent( Component* component );

	void setTag( const char* str );

	const char* getTag() const;
	Array<Component*>* getComponents();

private:
	char tag[ACTOR_TAG_MAX_LEN];
	Array<Component*> components;
};