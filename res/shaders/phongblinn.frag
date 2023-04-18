#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal; 
in vec3 FragPos;  

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	vec4 texColor = texture(texture_diffuse1, TexCoord);

	//ambient
	vec3 ambient = 0.05 * texColor.xyz;
	//diffuse
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 normal = normalize(Normal);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * texColor.xyz;
	//specular
	vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
	vec3 specular = vec3(0.3) * spec;

	FragColor = vec4(ambient + diffuse + specular, 1);
}
