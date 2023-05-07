#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal; 
in vec3 FragPos;  

uniform vec3 viewPos;
uniform vec3 color;

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
}