#version 330

layout(location=0) in vec3 vertPosition;
layout(location=1) in vec2 vertUV;
layout(location=2) in vec3 vertNormal;
layout(location=3) in vec3 vertTangent;
layout(location=4) in vec3 vertBitangent;

out vec2 geomUV;
out vec3 geomNormal;
out mat3 geomTBN;
out vec3 geomTangent;

uniform mat4 worldMatrices[100];

void main()
{
	gl_Position = worldMatrices[gl_InstanceID] * vec4( vertPosition, 1.0 );
	geomUV = vertUV;
	geomNormal = vertNormal;
	geomTangent = vertTangent;
	
	vec3 T = normalize( vec3( worldMatrices[gl_InstanceID] * vec4( vertTangent, 0.0 ) ) );
	vec3 B = normalize( vec3( worldMatrices[gl_InstanceID] * vec4( vertBitangent, 0.0 ) ) );
	vec3 N = normalize( vec3( worldMatrices[gl_InstanceID] * vec4( vertNormal, 0.0 ) ) );
	
	geomTBN = mat3( T, B, N );
}