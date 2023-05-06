#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoord;
in vec3 Normal; 
in vec3 FragPos;  

uniform vec3 viewPos;
uniform vec3 color;
uniform sampler2D texture_diffuse1;

//RimLight
uniform float n_r;

float classicRim(vec3 viewDir){
	return pow((1.f - clamp(dot(viewDir, Normal),0.0f,1.0f)), n_r);
}

float powerRim(vec3 viewDir){
	return 1-pow((1.f - clamp(dot(viewDir, Normal),0.0f,1.0f)), n_r);
}

void main()
{
	vec3 truColor = vec3(1,1,1) - color;
	vec3 viewDir = normalize(viewPos - FragPos);
	FragColor = vec4(1,1,1,2) - vec4(truColor * powerRim(viewDir), 1);
	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}