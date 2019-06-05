#version 330 core

in Data
{
	vec3 color;
	vec2 texCoord;
}FSIn;


out vec4 outColor;

uniform sampler2D colorTex;
uniform vec2 texCoordInc = vec2(0.001);
uniform float blur = 0.1f;
uniform int samples = 10;

float bias = 0.01f;
const float CoC = 0.03;
const int rings = 6;

float blurFunction(sampler2D texture, float i, float j, int ringsamples, inout vec3 col, float w, float h, float blur) {
        float rings2 = float(rings);
        float step = 3.1415926f*2.0 / float(ringsamples);
        float pw = cos(j*step)*i;
        float ph = sin(j*step)*i;
        float p = 1.0;
        col += texture2D(colorTex,FSIn.texCoord.xy + vec2(pw*w,ph*h) * blur).rgb * mix(1.0, i/rings2, bias) * p;
        return 1.0 * mix(1.0, i /rings2, bias) * p;
}

void main(void)
{
int maxringsamples = rings * samples;
vec3 col = texture2D(colorTex, FSIn.texCoord).rgb;
float s = 1.0;
int ringsamples;
for (int i = 1; i <= rings; i++)
{
    ringsamples = i * samples;
    for (int j = 0 ; j < maxringsamples ; j++)
    {
        if (j >= ringsamples) break;
        s += blurFunction(colorTex, float(i), float(j), ringsamples, col, texCoordInc.x, texCoordInc.y, blur);
    }
 }
 col /= s; //divide by sample count


    //blurColor /= sumWeights;

    outColor = vec4(col,1);
}
