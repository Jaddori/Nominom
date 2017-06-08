#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in vec2 geomUV[];
in vec3 geomNormal[];
in mat3 geomTBN[];

out vec2 fragUV;
out vec3 fragNormal;
out mat3 fragTBN;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 worldMatrix;
uniform vec3 cameraPosition;

void main()
{
	vec3 p1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 p2 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	
	vec3 triangleNormal = normalize( cross( p1, p2 ) );
	vec3 cameraNormal = normalize( cameraPosition - gl_in[1].gl_Position.xyz );
	
	if( dot( triangleNormal, cameraNormal ) > 0.0 )
	{
		for( int i=0; i<3; i++ )
		{
			gl_Position = projectionMatrix * viewMatrix * gl_in[i].gl_Position;
			fragUV = geomUV[i];
			fragNormal = geomNormal[i];
			fragTBN = geomTBN[i];
			
			EmitVertex();
		}
		
		EndPrimitive();
	}
}