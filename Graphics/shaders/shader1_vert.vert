#version 330 core 

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 uv;

uniform mat4x4 _Model;
uniform mat4x4 _View;
uniform mat4x4 _Projection;

out Data
{
	vec3 color;
	vec2 texCoord;
} VSOut;

void main(void)
{
gl_Position = _Projection * _View * _Model * vec4(position, 1);
VSOut.color = color;
VSOut.texCoord = uv;
}
