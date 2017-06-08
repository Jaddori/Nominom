#pragma once

#include "BaseIncludes.h"

struct DebugOBB
{
	glm::vec3 position;
	glm::vec3 xAxis;
	glm::vec3 yAxis;
	glm::vec3 zAxis;
	glm::vec3 extents;
	glm::vec4 color;
};