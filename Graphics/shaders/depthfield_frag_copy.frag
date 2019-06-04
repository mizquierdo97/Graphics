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


float znear =  1.0f;
float zfar = 300.0f;
float focalLength = 5.0f;
float focalDepth = 10.0f;
float fstop = 0.5f;
float dithering = 0.0001f;
float maxblur = 1.0f;
float threshold = 4.0f;
float gain = 0.0f;
float bias = 0.0f;
float fringe = 0.0f;
// constants TODO should be const-qualified
vec2 texel = vec2((texCoordInc.x),(texCoordInc.y));
float dbsize = 1.25;
const float CoC = 0.03;
const int rings = 6;
const int samples = 10;
const int maxringsamples = rings * samples;
// generating noise / pattern texture for dithering
vec2 rand(vec2 coord) {
        float noiseX = ((fract(1.0-coord.s*((1/texCoordInc.x)/2.0))*0.25)+(fract(coord.t*((1/texCoordInc.y)/2.0))*0.75))*2.0-1.0;
        float noiseY = ((fract(1.0-coord.s*((1/texCoordInc.x)/2.0))*0.75)+(fract(coord.t*((1/texCoordInc.y)/2.0))*0.25))*2.0-1.0;
        // if (noise) {
        // 	noiseX = clamp(fract(sin(dot(coord ,vec2(12.9898,78.233))) * 43758.5453),0.0,1.0)*2.0-1.0;
        // 	noiseY = clamp(fract(sin(dot(coord ,vec2(12.9898,78.233)*2.0)) * 43758.5453),0.0,1.0)*2.0-1.0;
        // }
        return vec2(noiseX,noiseY);
}
// processing the sample
vec3 color(vec2 coords,float blur) {
        vec3 col = vec3(0.0);
        // read from the render buffer at an offset
        col.r = texture2D(colorTex,coords + vec2(0.0,1.0)*texel*fringe*blur).r;
        col.g = texture2D(colorTex,coords + vec2(-0.866,-0.5)*texel*fringe*blur).g;
        col.b = texture2D(colorTex,coords + vec2(0.866,-0.5)*texel*fringe*blur).b;
        vec3 lumcoeff = vec3(0.299,0.587,0.114); // arbitrary numbers???
        float lum = dot(col.rgb, lumcoeff);
        float thresh = max((lum-threshold)*gain, 0.0);
        return col+mix(vec3(0.0),col,thresh*blur);
}
float gather(float i, float j, int ringsamples, inout vec3 col, float w, float h, float blur) {
        float rings2 = float(rings);
        float step = 3.1415926f*2.0 / float(ringsamples);
        float pw = cos(j*step)*i;
        float ph = sin(j*step)*i;
        float p = 1.0;
        col += color(FSIn.texCoord.xy + vec2(pw*w,ph*h), blur) * mix(1.0, i/rings2, bias) * p;
        return 1.0 * mix(1.0, i /rings2, bias) * p;
}
float linearizeDepth(float depth) {
        return -zfar * znear / (depth * (zfar - znear) - zfar);
}
void main(void)
{

        float depth = linearizeDepth(texture2D(depthTex, FSIn.texCoord).r);
        float f = focalLength; // focal length in mm,
        float d = focalDepth*1000.0; // focal plane in mm,
        float o = depth*1000.0; // depth in mm,
        float a = (o*f)/(o-f);
        float b = (d*f)/(d-f);
        float c = (d-f)/(d*fstop*CoC);
        float blur = clamp(abs(a-b)*c,0.0,1.0);
        // calculation of pattern for dithering
        vec2 noise = rand(FSIn.texCoord.xy)*dithering*blur;
        // getting blur x and y step factor
        float w = (texCoordInc.x)*blur*maxblur+noise.x;
        float h = (texCoordInc.y)*blur*maxblur+noise.y;
        // calculation of final color,
        vec3 col = texture2D(colorTex, FSIn.texCoord.xy).rgb;

        float s = 1.0;
        int ringsamples;
        for (int i = 1; i <= rings; i++)
        {
            ringsamples = i * samples;
            for (int j = 0 ; j < maxringsamples ; j++)
            {
                if (j >= ringsamples) break;
                s += gather(float(i), float(j), ringsamples, col, w, h, blur);
            }
         }
         col /= s; //divide by sample count

        outColor = vec4(col,1.0);
}
