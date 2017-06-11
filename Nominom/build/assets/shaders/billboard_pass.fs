#version 330

in vec2 fragUV;

out vec4 finalColor;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;

void main()
{
	//finalColor = texture( diffuseMap, fragUV );
	finalColor = vec4( 1.0, 0.0, 0.0, 1.0 );
}