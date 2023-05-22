#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal; 
in vec3 FragPos;  
in vec4 fragPosLight;

uniform sampler2D texture_diffuse1;
//uniform vec3 color;
uniform vec3 camPos;
uniform sampler2D depthMap;

void main()
{
	float shadow = 0.0f;

	vec3 lightCoords = fragPosLight.xyz / fragPosLight.w;
	if(lightCoords.z <= 1.0f)
	{
		lightCoords = (lightCoords + 1.0f) / 2.0f;
		
		float closestDepth = texture(depthMap, lightCoords.xy).r;
		float currentDepth = lightCoords.z;
		
		if (currentDepth > closestDepth)
			shadow = 1.0f;
	}
	vec4 texColor = texture(texture_diffuse1, TexCoord)* (1.0f - shadow);
    if(texColor.a < 0.1)
        discard;
	FragColor = texColor;
	
}
