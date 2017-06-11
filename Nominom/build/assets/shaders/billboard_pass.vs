#version 330

layout(location=0) in vec3 vertPosition;
layout(location=1) in vec2 vertSize;

out vec2 geomSize;

void main()
{
	gl_Position = vec4( vertPosition, 1.0 );
	geomSize = vertSize;
}