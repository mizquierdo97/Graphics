#version 330 core

in Data
{
        vec3 color;
        vec2 texCoord;
}FSIn;


out vec4 outColor;


uniform sampler2D deferredTex;
uniform sampler2D SSAOTex;
uniform sampler2D SSAOBlurTex;
uniform sampler2D blurTex;
uniform sampler2D depthFieldTex;
uniform sampler2D colorTex;
uniform sampler2D normalTex;
uniform sampler2D depthTex;

uniform int type = 0;

void main(void)
{
    //SSAO + DEFERRED + DEPTH FIELD
    if(type == 0)
    {
        float occlussion = texture2D(SSAOBlurTex, FSIn.texCoord).r;
        outColor = texture2D(depthFieldTex, FSIn.texCoord) * occlussion;
    }
    //DEFERRED
    else if(type == 1)
    {
        outColor = texture2D(deferredTex, FSIn.texCoord);
    }
    //COLOR
    else if(type == 2)
    {
        outColor = texture2D(colorTex, FSIn.texCoord);
    }
    //NORMAL
    else if(type == 3)
    {
        outColor = texture2D(normalTex, FSIn.texCoord);
    }
    //DEPTH
    else if(type == 4)
    {
        outColor = texture2D(depthTex, FSIn.texCoord);
    }
    //SSAO + DEFERRED
    else if(type == 5)
    {
        float occlussion = texture2D(SSAOTex, FSIn.texCoord).r;
        outColor = texture2D(deferredTex, FSIn.texCoord) * occlussion;
    }
    //SSAO ONLY
    else if(type == 6)
    {
        outColor = texture2D(SSAOBlurTex, FSIn.texCoord);
    }
    //BLUR
    else if(type == 7)
    {
        outColor = texture2D(blurTex, FSIn.texCoord);
    }

}
