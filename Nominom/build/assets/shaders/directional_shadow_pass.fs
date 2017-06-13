#version 330

out vec4 finalColor;

uniform float farPlane;
uniform float nearPlane;

void main()
{
	float z = gl_FragCoord.z;
	finalColor = vec4( z, z, z, 1.0 );
}