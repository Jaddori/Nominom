#pragma once

#include <stdint.h>
#include <assert.h>
#include <cstdlib>
#include <math.h>
#include <sstream>
#include "Array.h"

#include "lua.hpp"

#ifdef WIN32
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

#include "GL\glew.h"
#include "SDL\SDL.h"

#else
#include <glm.hpp>
#include <matrix_transform.hpp>
#include <type_ptr.hpp>
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <SDL2/SDL.h>
#endif
