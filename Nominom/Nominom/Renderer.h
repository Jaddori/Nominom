#pragma once

#include "BaseIncludes.h"
#include "ModelInstance.h"
#include "Assets.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	void queue( Array<ModelInstance>* instances );

	void render( Assets* assets );

private:
	Array<ModelInstance>* instances;
};