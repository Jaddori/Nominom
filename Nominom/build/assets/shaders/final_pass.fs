#version 330

in vec2 fragUV;

out vec4 finalColor;

uniform sampler2D lightTarget;
uniform sampler2D billboardTarget;
uniform sampler2D billboardAlphaTarget;

void main()
{
	vec4 ftColor = texture( lightTarget, fragUV );
	vec4 btColor = texture( billboardTarget, fragUV );
	float btAlpha = texture( billboardAlphaTarget, fragUV ).r;
	
	//float lightFactor = 1.0 - btColor.a;
	float lightFactor = 1.0 - btAlpha;
	
	finalColor = ( ftColor * lightFactor ) + btColor;
	//finalColor = ftColor;
	//finalColor = btColor;
}