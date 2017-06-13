#version 330

layout(location=0) in vec3 vertPosition;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 worldMatrices[100];

void main()
{
	gl_Position = projectionMatrix * viewMatrix * worldMatrices[gl_InstanceID] * vec4( vertPosition, 1.0 );
}