#version 430

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal; // On récupère la normale !

out flat vec3 vertexColor;
out flat vec3 vertexNormal; // On la transmet intacte (sans l'interpoler)

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vertexColor = aColor;
    vertexNormal = aNormal;
    // Plus besoin de FragPos !
    gl_Position = projection * view * vec4(aPos, 1.0);
}