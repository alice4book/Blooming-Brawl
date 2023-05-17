#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal; 
in vec3 FragPos;  

uniform sampler2D texture_diffuse1;
uniform vec3 viewPos;
uniform vec3 hlcolor;


void main()
{
	FragColor = texture(texture_diffuse1, TexCoord) + vec4(hlcolor,0);
}