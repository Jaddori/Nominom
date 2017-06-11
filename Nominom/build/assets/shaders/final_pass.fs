#version 330

in vec2 fragUV;

out vec4 finalColor;

uniform sampler2D finalTarget;
uniform sampler2D billboardTarget;

void main()
{
	vec4 ftColor = texture( finalTarget, fragUV );
	vec4 btColor = texture( billboardTarget, fragUV );
	
	ftColor.a = 1.0 - btColor.a;
	
	finalColor = ftColor + btColor;
	//finalColor = vec4( 1.0, 0.0, 0.0, 1.0 );
}