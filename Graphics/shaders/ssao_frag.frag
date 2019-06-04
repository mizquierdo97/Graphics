#version 330 core

in Data
{
	vec3 color;
	vec2 texCoord;
}FSIn;


out vec4 outColor;

uniform sampler2D colorTex;
uniform sampler2D normalTex;
uniform sampler2D depthTex;
uniform sampler2D rotationVectorsTex;
uniform int type = 0;
uniform vec2 texCoordInc = vec2(0.001);
uniform vec3 samples[64];

uniform mat4 view;
uniform mat4 proj;
uniform vec3 camPos;;

vec3 GetViewFragPos(float depth)
{
    vec3 fragPos;
    fragPos.x = FSIn.texCoord.x * 2 - 1;
    fragPos.y = FSIn.texCoord.y * 2 - 1;
    fragPos.z = depth * 2 - 1;

    vec4 fragView = inverse(proj) * vec4(fragPos.x,fragPos.y,fragPos.z,1);
    fragView = fragView/fragView.w;
    return fragView.xyz;
}
void main(void)
{

    vec3 blurColor = vec3(0,0,0);
    vec3 color = texture(colorTex, FSIn.texCoord).rgb;
    vec3 normal = texture(normalTex, FSIn.texCoord).rgb * 2 - 1;
    float depth = texture(depthTex, FSIn.texCoord).r;
    vec2 noiseScale = (1/texCoordInc) / textureSize(rotationVectorsTex, 0);
    vec3 randomVec = texture(rotationVectorsTex, FSIn.texCoord * noiseScale).rgb;

    vec3 tangent = normalize(randomVec - normal * cross(normal, vec3(0,1,0)));
    //tangent = normalize(cross(normal.xyz, vec3(0,1,0)));
    vec3 bitangent = normalize(cross(normal, tangent));
    mat3 TBN = mat3(tangent, bitangent, normal);
    float occlussion = 0.0f;
    for(int i = 0; i <  64; i++)
    {
        vec3 p = TBN * samples[i];
        vec4 v = inverse(view) * vec4(p.xyz, 0.0f);
        float radius = 0.5f;
        vec3 s = GetViewFragPos(depth) + v.xyz * radius;
        vec4 sampleTextCoord = proj * vec4(s, 1.0f);
        sampleTextCoord.xyz/= sampleTextCoord.w;
        sampleTextCoord.xyz = sampleTextCoord.xyz * 0.5f + 0.5f;
        float sampledDepth = texture(depthTex, sampleTextCoord.xy).r;
        vec3 sampledPosView = GetViewFragPos(sampledDepth);

        float rangeCheck = smoothstep(0.0, 1.0, radius/abs(sampledPosView.z -s.z));
        rangeCheck *= rangeCheck;

        occlussion += (s.z < sampledPosView.z - 0.01 ? 1.0 : 0.0) * rangeCheck;
}
    occlussion = occlussion/64;
    occlussion = occlussion*occlussion;
float o = (1.0f - occlussion);
outColor =vec4(o,o,o,1);
}
