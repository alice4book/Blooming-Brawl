#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal; 
in vec3 FragPos;  

uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;
uniform vec3 color;

void main()
{
	float ambientStrength = 0.5f;
    vec3 ambient = ambientStrength * color;
	vec3 result = texture(texture_diffuse1, TexCoord).xyz * ambient;
	FragColor = vec4(result, 1.0);
}