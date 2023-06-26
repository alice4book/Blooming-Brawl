#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{    
	vec4 texColor = vec4(textColor.r, textColor.g, textColor.b, 1.0);
    if(texture(text, TexCoords).r < 0.1)
        discard;
	FragColor = texColor;
}