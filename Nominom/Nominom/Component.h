#pragma once

#include "Actor.h"

class Component
{
public:
	Component();
	virtual ~Component();

	void setParent( Actor* actor );

	Actor* getParent();

protected:
	Actor* parent;
};