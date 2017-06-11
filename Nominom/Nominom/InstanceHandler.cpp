#include "InstanceHandler.h"

InstanceHandler::InstanceHandler()
{
	LOG( VERBOSITY_INFORMATION, "InstanceHandler", "Constructing." );
}

InstanceHandler::~InstanceHandler()
{
	LOG( VERBOSITY_INFORMATION, "InstanceHandler", "Destructing." );
}

InstanceIndex InstanceHandler::add( int mesh, int diffuseMap, int normalMap, int specularMap )
{
	LOG( VERBOSITY_INFORMATION, "InstanceHandler(add)", "Adding new instance." );

	InstanceIndex result = {};

	ModelInstance newInstance( mesh, diffuseMap, normalMap, specularMap );
	int index = instances.find( newInstance );

	if( index < 0 )
	{
		result.instance = instances.getSize();
		instances.add( newInstance );
	}

	result.worldMatrix = instances[result.instance].add();
	return result;
}

ModelInstance* InstanceHandler::getInstance( int index )
{
	assert( index >= 0 && index < instances.getSize() );
	return &instances[index];
}

Array<ModelInstance>* InstanceHandler::getInstances()
{
	return &instances;
}