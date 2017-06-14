#version 330

in vec4 fragColor;

out vec4 finalColor;

uniform vec2 screenSize;
uniform sampler2D depthTarget;

void main()
{
	vec2 uv = gl_FragCoord.xy / screenSize;
	float depth = texture( depthTarget, uv ).r;

	float fragDepth = gl_FragCoord.z;
	
	if( fragDepth > depth )
		discard;
	
	finalColor = fragColor;
}