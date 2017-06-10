#include "Actor.h"
#include "Component.h"

Actor::Actor()
{
	tag[0] = 0;
}

Actor::~Actor()
{
}

void Actor::addComponent( Component* component )
{
	components.add( component );
	component->setParent( this );
}

void Actor::setTag( const char* str )
{
	int len = strlen( str );
	assert( len < ACTOR_TAG_MAX_LEN-1 );

	strcpy( tag, str );
	tag[ACTOR_TAG_MAX_LEN-1] = 0;
}

const char* Actor::getTag() const
{
	return tag;
}

Array<Component*>* Actor::getComponents()
{
	return &components;
}