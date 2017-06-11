#version 330

const vec2 QUAD_CORNERS[4] = vec2[]( vec2(-0.5, -0.5), vec2(-0.5, 0.5), vec2(0.5, -0.5), vec2(0.5, 0.5) );
const vec2 UV_CORNERS[4] = vec2[]( vec2(0.0, 1.0), vec2(0.0, 0.0), vec2(1.0, 1.0), vec2(1.0, 0.0) );

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in vec2 geomSize[];

out vec2 fragUV;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main()
{
	for( int i=0; i<4; i++ )
	{
		vec4 p = viewMatrix * gl_in[0].gl_Position;
		p.xy += QUAD_CORNERS[i] * geomSize[0];
		gl_Position = projectionMatrix * p;
		fragUV = UV_CORNERS[i];
		EmitVertex();
	}
	
	EndPrimitive();
}