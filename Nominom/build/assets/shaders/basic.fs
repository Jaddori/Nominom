#version 330

in vec2 fragUV;
in vec3 fragNormal;
in vec3 fragTangent;
in vec3 fragBitangent;

out vec4 finalColor;

uniform sampler2D diffuseMap;

void main()
{
	finalColor = texture( diffuseMap, fragUV );
}