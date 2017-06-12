#version 330

layout(location=0) in vec2 vertPosition;
layout(location=1) in vec2 vertSize;
layout(location=2) in vec2 vertStartUV;
layout(location=3) in vec2 vertEndUV;
layout(location=4) in vec4 vertColor;

out vec2 geomStartUV;
out vec2 geomEndUV;
out vec2 geomSize;
out vec4 geomColor;

void main()
{
	gl_Position = vec4( vertPosition, 0.0, 1.0 );
	geomStartUV = vertStartUV;
	geomEndUV = vertEndUV;
	geomSize = vertSize;
	geomColor = vertColor;
	
	//geomStartUV = vec2( 0.0, 0.0 );
	//geomEndUV = vec2( 1.0, 1.0 );
}