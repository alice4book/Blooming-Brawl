#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoord;
out vec3 Normal; 
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main()
{
    // Calculate the displacement factor based on time
    float displacement = sin(time) * 0.06; // Adjust the factor to control the amplitude of the movement

    // Modify the vertex position
    vec4 displacedPosition = vec4(aPos.x, aPos.y + displacement, aPos.z, 1.0);

    // Transform the vertex position
    gl_Position = projection * view * model * displacedPosition;

    // Pass along other data to the fragment shader
	
    FragPos = vec3(model * displacedPosition);
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    TexCoord = aTexCoords;  
}