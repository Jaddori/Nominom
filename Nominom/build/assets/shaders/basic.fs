#version 330

in vec2 fragUV;
in vec3 fragNormal;
in vec3 fragTangent;
in vec3 fragBitangent;

out vec4 fragColor;

uniform sampler2D diffuseMap;

void main()
{
	fragColor = texture( diffuseMap, fragUV );
}