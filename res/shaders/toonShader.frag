#version 330 core

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 fragPosLight;


uniform sampler2D texture_diffuse1;
uniform sampler2D depthMap;
uniform vec3 viewPos;
uniform DirLight dirLight;
uniform float gamma;

void main()
{
	float shadow = 0.0f;
	
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
	//vec3 lightDir = normalize(vec3(-(-0.2f, -1.0f, -0.3f)));
	vec3 lightDir = -normalize(vec3(1,  -1, 1));
    // diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 500.0f);//100
	
	vec3 lightCoords = fragPosLight.xyz / fragPosLight.w;
	if(lightCoords.z <= 1.0f)
	{
		lightCoords = (lightCoords + 1.0f) / 2.0f;
		
		float closestDepth = texture(depthMap, lightCoords.xy).r;
		float currentDepth = lightCoords.z;
		float bias = 0.0005f;
		if (currentDepth > closestDepth+bias)
			shadow += 0.5f;

	}

	//toon shading
	vec3 ambientColor = {0.4,0.4,0.4};
	//float NdotL = dot(norm, lightDir);
	float lightIntensity = 0;
	
	if (diff != 0)
	{
		if (diff <= 0.5) lightIntensity = smoothstep(0.0, 0.03, diff)/2;
		else lightIntensity = smoothstep(0.5, 0.53, diff)/2 + 0.5;
	}
	
	//lightIntensity = smoothstep(0, 0.01, lightIntensity);

	//float lightIntensity = NdotL > 0 ? 1 : 0;
	
	
    // combine results
    vec3 ambient =  vec3(1.0f, 1.0f, 1.0f) * 0.3f * vec3(texture(texture_diffuse1, TexCoord));
    vec3 diffuse = vec3(1.0f, 1.0f, 1.0f) * 1.0f * diff * vec3(texture(texture_diffuse1, TexCoord)) * (1.0f - shadow);
    vec3 specular = vec3(0.2f, 0.2f, 0.2f) * 1.0f * spec * (1.0f - shadow);
	/*
	vec3 specularColor = {0.9,0.9,0.9};
	float specularIntensitySmooth = smoothstep(0.005, 0.01, spec);
	vec3 specular_toon = specularIntensitySmooth * specularColor;
	*/

	float rimAmount = 0.8;
	vec3 rimColor = {1,1,1};
	float rimDot = 1 - dot(viewDir, norm);
	float rimIntensity = smoothstep(rimAmount - 0.02, rimAmount + 0.02, rimDot);
	vec3 rim = rimIntensity * rimColor;
	
	//vec3 color = vec3(ambient + diffuse + specular);
	diffuse = vec3(texture(texture_diffuse1, TexCoord)) * (1.0f - shadow);
	vec3 color = diffuse;
	color = color * (lightIntensity + ambientColor + rim); //+ specular_toon);

	//color = pow(color, vec3(1.0/gamma));
    FragColor = vec4(color, 1.0);
	
	
}
