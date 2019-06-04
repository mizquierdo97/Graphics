#version 330 core

in Data
{
	vec3 color;
	vec2 texCoord;
}FSIn;


out vec4 outColor;

uniform sampler2D colorTex;
uniform sampler2D normalMap;
uniform int type = 0;
uniform vec2 texCoordInc = vec2(0.001);
float fringe = 0.0f;
float threshold = 4.0f;
float bias = 0.01f;
float gain = 0.01f;
float dbsize = 1.25;
const float CoC = 0.03;
const int rings = 6;
const int samples = 10;
const int maxringsamples = rings * samples;

vec3 colorFunc(sampler2D texture, vec2 coords,float blur) {
        vec3 col = vec3(0.0);
        // read from the render buffer at an offset
        col.r = texture2D(texture,coords + vec2(0.0,1.0)*texCoordInc*fringe*blur).r;
        col.g = texture2D(texture,coords + vec2(-0.866,-0.5)*texCoordInc*fringe*blur).g;
        col.b = texture2D(texture,coords + vec2(0.866,-0.5)*texCoordInc*fringe*blur).b;
        vec3 lumcoeff = vec3(0.299,0.587,0.114); // arbitrary numbers???
        float lum = dot(col.rgb, lumcoeff);
        float thresh = max((lum-threshold)*gain, 0.0);
        return col+mix(vec3(0.0),col,thresh*blur);
}
float gather(sampler2D texture, float i, float j, int ringsamples, inout vec3 col, float w, float h, float blur) {
        float rings2 = float(rings);
        float step = 3.1415926f*2.0 / float(ringsamples);
        float pw = cos(j*step)*i;
        float ph = sin(j*step)*i;
        float p = 1.0;
        col += colorFunc(texture, FSIn.texCoord.xy + vec2(pw*w * blur,ph*h * blur), blur) * mix(1.0, i/rings2, bias) * p;
        return 1.0 * mix(1.0, i /rings2, bias) * p;
}

void main(void)
{

vec3 col = texture2D(colorTex, FSIn.texCoord).rgb;
float s = 1.0;
int ringsamples;
for (int i = 1; i <= rings; i++)
{
    ringsamples = i * samples;
    for (int j = 0 ; j < maxringsamples ; j++)
    {
        if (j >= ringsamples) break;
        s += gather(colorTex, float(i), float(j), ringsamples, col, texCoordInc.x, texCoordInc.y, 0.1f);
    }
 }
 col /= s; //divide by sample count


    //blurColor /= sumWeights;

    outColor = vec4(col,1);
}
