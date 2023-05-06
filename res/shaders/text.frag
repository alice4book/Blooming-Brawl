#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoords;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{    
	vec4 texColor = vec4(1.0, 1.0, 1.0, 1.0);
	float brightness = dot(texColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(texture(text, TexCoords).r < 0.1){
		BrightColor = vec4(0.0, 0.0, 0.0, 0.0);
        discard;
	}

	FragColor = texColor;
	if(brightness > 1.2)
		BrightColor = vec4(texColor.rgb, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 0.0);
}