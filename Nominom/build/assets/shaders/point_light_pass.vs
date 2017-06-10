#version 330

layout(location=0) in vec3 vertPosition;
//layout(location=0) in vec2 vertPosition;
layout(location=1) in vec2 vertUV;

//out vec2 fragUV;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 worldMatrix;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * worldMatrix * vec4( vertPosition, 1.0 );
	//gl_Position = vec4( vertPosition, 0.0, 1.0 );
	//fragUV = vertUV * vec2(1.0, -1.0);
}