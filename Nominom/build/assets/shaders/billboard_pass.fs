#version 330

in vec2 fragUV;
in vec4 fragPosition;
in mat3 fragTBN;

//out vec4 finalColor;
layout(location=0) out vec4 finalDiffuse;
layout(location=1) out vec4 finalPosition;
layout(location=2) out vec4 finalNormal;
layout(location=3) out vec4 finalAlpha;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;

void main()
{
	//finalColor = texture( diffuseMap, fragUV );
	
	finalDiffuse = texture( diffuseMap, fragUV );
	finalPosition = fragPosition;
	
	vec3 normal = texture( normalMap, fragUV ).xyz;
	normal = normalize( normal * 2.0 - 1.0 );
	normal = normalize( fragTBN * normal );
	finalNormal = vec4( normal, 1.0 );
	
	finalAlpha = vec4( finalDiffuse.a, 0.0, 0.0, 1.0 );
}