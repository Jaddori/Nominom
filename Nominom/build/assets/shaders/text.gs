#version 330

layout(points) in;
layout(triangle_strip, max_vertices=4) out;

in vec2 geomStartUV[];
in vec2 geomEndUV[];
in vec2 geomSize[];
in vec4 geomColor[];

out vec2 fragUV;
out vec4 fragColor;

uniform mat4 projectionMatrix;

void main()
{
	fragColor = geomColor[0];

	gl_Position = projectionMatrix * gl_in[0].gl_Position;
	fragUV = geomStartUV[0];
	EmitVertex();
	
	gl_Position = projectionMatrix * ( gl_in[0].gl_Position + vec4( 0.0, geomSize[0].y, 0.0, 0.0 ) );
	fragUV = vec2( geomStartUV[0].x, geomEndUV[0].y );
	EmitVertex();
	
	gl_Position = projectionMatrix * ( gl_in[0].gl_Position + vec4( geomSize[0].x, 0.0, 0.0, 0.0 ) );
	fragUV = vec2( geomEndUV[0].x, geomStartUV[0].y );
	EmitVertex();
	
	gl_Position = projectionMatrix * ( gl_in[0].gl_Position + vec4( geomSize[0], 0.0, 0.0 ) );
	fragUV = geomEndUV[0];
	EmitVertex();
	
	EndPrimitive();
}