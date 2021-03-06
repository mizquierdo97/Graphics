#version 330 core

in Data
{
	vec3 color;
	vec2 texCoord;
}FSIn;


layout(location = 0)out vec4 outColor;
layout(location = 1) out vec4 normals;

uniform sampler2D tex;

void main(void)
{
	outColor = texture(tex, FSIn.texCoord);
        normals  = vec4(FSIn.color, 1);
}
