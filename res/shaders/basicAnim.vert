#version 460 core
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
    const vec4 pos = vec4(aPos, 1.0f);
    const vec4 norm = vec4(aNormal, 0.0f);

    vec4 posSkinned = vec4(0.0f);
    vec4 normSkinned = vec4(0.0f);
    
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        if(aBoneIds[i] >= 0)
        {
            const mat4 bone = finalBonesMatrices[aBoneIds[i]];
            const float weight = aWeights[i];

            posSkinned += (bone * pos) * weight;
            normSkinned += (bone * norm) * weight;

        }
    }
    
    posSkinned.w = 1.0f;

    vec4 worldPos = model * posSkinned;

    Normal = mat3(transpose(inverse(model))) * vec3(normSkinned);
    FragPos = vec3(worldPos);
    TexCoord = aTexCoords;   
	ourColor = aColor;	
    worldPos = view * worldPos;
	fragPosLight = lightProjection * vec4(FragPos, 1.0f);
    gl_Position = projection * worldPos;
}
