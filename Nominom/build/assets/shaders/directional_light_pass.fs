#version 330

struct DirectionalLight
{
	vec3 direction;
	vec3 color;
	float intensity;
};

in vec2 fragUV;

out vec4 finalColor;

uniform DirectionalLight directionalLight;
uniform vec3 cameraPosition;
uniform sampler2D diffuseTarget;
uniform sampler2D normalTarget;
uniform sampler2D positionTarget;
uniform sampler2D depthTarget;

vec4 calculateDirectionalLight( vec3 normal, vec3 position )
{
	float diffuseFactor = dot( normal, -directionalLight.direction );
	vec4 diffuseColor = vec4( 0.0, 0.0, 0.0, 1.0 );
	vec4 specularColor = vec4( 0.0, 0.0, 0.0, 0.0 );
	
	if( diffuseFactor > 0.0 )
	{
		diffuseColor.rgb = directionalLight.color * directionalLight.intensity * diffuseFactor;
		
		vec3 directionToEye = normalize( cameraPosition - position );
		vec3 halfDirection = normalize( directionToEye - directionalLight.direction );
		
		float specularFactor = dot( halfDirection, normal );
		specularFactor = pow( specularFactor, 8.0 ); // TEMP
		
		if( specularFactor > 0.0 )
		{
			specularColor.rgb = directionalLight.color * 2.0 * specularFactor;
		}
	}
	
	return diffuseColor + specularColor;
}

void main()
{
	vec3 diffuse = texture( diffuseTarget, fragUV ).rgb;
	vec3 position = texture( positionTarget, fragUV ).rgb;
	vec3 normal = texture( normalTarget, fragUV ).rgb;
	
	finalColor = vec4( diffuse, 1.0 ) * calculateDirectionalLight( normal, position );
}