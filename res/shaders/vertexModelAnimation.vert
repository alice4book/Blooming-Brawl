#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout(location = 5) in ivec4 aBoneIds; 
layout(location = 6) in vec4 aWeights;

out vec2 TexCoord;
out vec3 Normal; 
out vec3 FragPos;
out vec4 fragPosLight;
out vec4 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightProjection;
uniform vec4 aColor;
	
const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

void main()
{

    //Normal = mat3(transpose(inverse(model))) * aNormal;  

    vec4 totalPosition = vec4(0.0f);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        if(aBoneIds[i] == -1) 
            continue;
        if(aBoneIds[i] >= MAX_BONES) 
        {
            totalPosition = vec4(aPos,1.0f);
            break;
        }
        vec4 localPosition = finalBonesMatrices[aBoneIds[i]] * vec4(aPos,1.0f);
        totalPosition += localPosition * aWeights[i];
        vec3 localNormal = mat3(finalBonesMatrices[aBoneIds[i]]) * Normal;
    }
		
    mat4 viewModel = view * model;
    gl_Position =  projection * viewModel * totalPosition;

    FragPos = vec3(model * vec4(aPos, 1.0f));
    TexCoord = aTexCoords;   
	ourColor = aColor;	
	fragPosLight = lightProjection * vec4(FragPos, 1.0f);
}
