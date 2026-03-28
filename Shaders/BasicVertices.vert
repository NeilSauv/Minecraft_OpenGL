#version 430

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in float aFace;
layout (location = 3) in vec3 aOffset;
layout (location = 4) in ivec4 aRender;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(std430, binding = 0) readonly buffer BlockPatterns {
    int blockPatterns[];
};
uniform ivec2 atlasGrid;

void main()
{
    if (aRender.y == -1) {
        gl_Position = vec4(0.0);
        return;
    }

    vec3 pos = vec3(aPos.x, aPos.y, aPos.z);

    int blockType = aRender.x;
    int faceIndex = int(aFace);

    int tileIndex = blockPatterns[blockType * 6 + faceIndex];

    int tileX = tileIndex % atlasGrid.x;
    int tileY = tileIndex / atlasGrid.x;

    vec2 baseUV = vec2(tileX, tileY) / vec2(atlasGrid);
    TexCoord = baseUV + aTexCoord / vec2(atlasGrid);

    gl_Position = projection * view * model * vec4(pos + aOffset, 1.0);
}