#version 330

in vec2 fragUV;
in vec4 fragColor;

out vec4 finalColor;

uniform sampler2D diffuseMap;

void main()
{
	finalColor = texture( diffuseMap, fragUV ) * fragColor;
	
	// TODO: Figure out if this is slow
	if( finalColor.a <= 0.1 )
		discard;
}