#version 330 core

in Data
{
	vec3 color;
	vec2 texCoord;
}FSIn;


out vec4 outColor;

uniform sampler2D colorTex;
uniform sampler2D normalMap;

void main(void)
{
    vec3 dir = vec3(1,1,1);

vec3 color = texture(colorTex, FSIn.texCoord).rgb;
vec3 normal = texture(normalMap, FSIn.texCoord).rgb;

vec3 r = normalize(2 * dot(dir, normal) * normal - dir);
    vec3 v = vec3(0,0,1);

    float a = dot(r, v);
    float specular = max(pow(a, 100), 0);
    float intensity = max(0,dot(normal,dir));
    outColor = vec4(max(color * 0.2 ,color * intensity) + specular * 0.5,1);
}
