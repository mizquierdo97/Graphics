#version 330 core

in Data
{
	vec3 color;
	vec2 texCoord;
}FSIn;


out vec4 outColor;

uniform sampler2D colorTex;
uniform sampler2D normalMap;
uniform sampler2D depthTex;
uniform int type = 0;
uniform vec2 texCoordInc = vec2(0.001);


vec2 rand(vec2 coord) {
        float noiseX = ((fract(1.0-coord.s*((1/texCoordInc.x)/2.0))*0.25)+(fract(coord.t*((1/texCoordInc.y)/2.0))*0.75))*2.0-1.0;
        float noiseY = ((fract(1.0-coord.s*((1/texCoordInc.x)/2.0))*0.75)+(fract(coord.t*((1/texCoordInc.y)/2.0))*0.25))*2.0-1.0;
        // if (noise) {
        // 	noiseX = clamp(fract(sin(dot(coord ,vec2(12.9898,78.233))) * 43758.5453),0.0,1.0)*2.0-1.0;
        // 	noiseY = clamp(fract(sin(dot(coord ,vec2(12.9898,78.233)*2.0)) * 43758.5453),0.0,1.0)*2.0-1.0;
        // }
        return vec2(noiseX,noiseY);
}

void main(void)
{
float weights[11];
weights[0] =  0.035822;
weights[1] =  0.05879;
weights[2] =  0.086425;
weights[3] =  0.113806;
weights[4] =  0.13424;
weights[5] =  0.141836;
weights[6] =  0.13424;
weights[7] =  0.113806;
weights[8] =  0.086425;
weights[9] =  0.05879;
weights[10] = 0.035822;

vec3 blurColor = vec3(0,0,0);
vec3 color = texture(colorTex, FSIn.texCoord).rgb;
float depth = texture(depthTex, FSIn.texCoord).r ;

float far = 300.0f;
float near = 1.0f;
float focusDist = 30.0f;
float distNorm = (focusDist - near) / (far - near);
float dist = 0.01f;
float ld = ((2.0 * near) / (far + near - (2.0 * depth - 1.0) * (far - near)));
vec2 uv = vec2(0,0);
float depthMult = min(1,(distNorm - ld)*5);

float f = 10; // focal length in mm,
float d = 2.5*1000.0; // focal plane in mm,
float o = ld*1000.0; // depth in mm,
float a = (o*f)/(o-f);
float b = (d*f)/(d-f);
float c = (d-f)/(d*0.5*0.03);
float blur = clamp(abs(a-b)*c,0.0,1.0);

vec2 noise = rand(FSIn.texCoord.xy)*0.0001*blur;

if(type == 0)
{
uv = vec2(FSIn.texCoord.x - (texCoordInc.x * depthMult), FSIn.texCoord.y);
}
else
{
uv = vec2(FSIn.texCoord.x, FSIn.texCoord.y - (texCoordInc.y * depthMult));
}
float sumWeights = 0.0f;

for(int i = 0; i < 11; i++)
{
float nearDepth = texture(depthTex, uv).r;
  if(!(depth <= nearDepth))
{
    blurColor += texture(colorTex, uv).rgb * weights[i];
    sumWeights += weights[i];
}


if(type == 0)
uv += vec2(texCoordInc.x * depthMult, 0);
else
uv += vec2(0, texCoordInc.y * depthMult);


}

    outColor = vec4(blurColor / sumWeights,1);
    //outColor = vec4(depthMult,depthMult,depthMult,1);
    outColor = vec4(noise.x, noise.y, 0,1);
    /*
if(ld > (distNorm - dist) && ld < (distNorm + dist))
{
    outColor = vec4(1,0,0,1);
}
else
{
    outColor = vec4(vec3(ld,ld,ld),1);
}*/
}
