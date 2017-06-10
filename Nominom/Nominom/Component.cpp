#include "Component.h"

Component::Component()
{
}

Component::~Component()
{
}

void Component::setParent( Actor* actor )
{
	parent = actor;
}

Actor* Component::getParent()
{
	return parent;
}