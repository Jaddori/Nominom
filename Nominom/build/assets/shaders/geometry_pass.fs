#version 330

in vec2 fragUV;
in vec3 fragNormal;
in mat3 fragTBN;
in vec3 fragTangent;

layout(location=0) out vec4 finalDiffuse;
layout(location=1) out vec4 finalNormal;
layout(location=2) out vec4 finalSpecular;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;

void main()
{
	vec2 uv = vec2( fragUV.s, 1 - fragUV.t );
	
	finalDiffuse = texture( diffuseMap, uv );
	
	vec3 normal = texture( normalMap, uv ).rgb;
	normal = normalize( normal * 2.0 - 1.0 );
	normal = normalize( fragTBN * normal );
	finalNormal = vec4( normal, 1.0 );
	finalNormal = vec4( fragTangent, 1.0 );
	
	finalSpecular = texture( specularMap, uv );
}