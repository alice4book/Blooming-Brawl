#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal; 
in vec3 FragPos;  

uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;
//uniform vec3 color;

//RimLight
uniform float n_r;

float classicRim(vec3 viewDir){
	return pow((1.f - clamp(dot(viewDir, Normal),0.0f,1.0f)), n_r);
}

void main()
{
	vec3 viewDir = normalize(viewPos - FragPos);
	FragColor = texture(texture_diffuse1, TexCoord) *
		classicRim(viewDir);
	//texture(texture_diffuse1, TexCoord); //* vec4(color, 1);
}