#version 330

layout (location=0) in vec3 vertPosition;
layout (location=1) in vec2 vertUV;
layout (location=2) in vec3 vertNormal;
layout (location=3) in vec3 vertTangent;
layout (location=4) in vec3 vertBitangent;

out vec2 fragUV;
out vec3 fragNormal;
out vec3 fragTangent;
out vec3 fragBitangent;

uniform mat4 worldMatrices[100];
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * worldMatrices[gl_InstanceID] * vec4( vertPosition, 1.0 );
	fragUV = vertUV;
	fragNormal = vertNormal;
	fragTangent = vertTangent;
	fragBitangent = vertBitangent;
}