#version 430
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in float aFace;
layout (location = 3) in vec3 aOffset;
layout (location = 4) in vec4 aRender;
layout (location = 5) in int aFaceIndex;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int blockPatterns[42];

void main()
{
    if(aRender.y == -1)
    {
        return;
    }

    vec3 pos;

    if(aRender.y != 0 && aRender.y != 1 || aRender.z != 1)
    {

        if(aRender.w == 0)
            pos = vec3(- aPos.x, aPos.y,- aPos.z);
        else if(aRender.w == 1)
            pos = vec3(aPos.x, aPos.y,aPos.z);
        else if(aRender.w == 2)
            pos = vec3(- aPos.z, aPos.y,- aPos.x);
        else
            pos = vec3(aPos.z, aPos.y,aPos.x);
    }
    else
    {
        pos = vec3(aPos.z, aPos.y,aPos.x);
    }

    float y = 0;//blockPatterns[7*6+int(aFace)]/25.0;
    float x = blockPatterns[6*int(aRender.x)+int(aFace)];

    gl_Position =  projection * view * model * vec4(pos + aOffset, 1.0);
    float u = x/25.0 + aTexCoord.x /25.0;
    float v = y/25.0 + aTexCoord.y /25.0;

    TexCoord = vec2(u,v);
}
