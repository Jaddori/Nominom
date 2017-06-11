#version 330

in vec2 fragUV;

out vec4 finalColor;

uniform sampler2D lightTarget;
uniform sampler2D billboardTarget;

void main()
{
	vec4 ftColor = texture( lightTarget, fragUV );
	vec4 btColor = texture( billboardTarget, fragUV );
	
	float lightFactor = 1.0 - btColor.a;
	
	finalColor = ( ftColor * lightFactor ) + btColor;
}