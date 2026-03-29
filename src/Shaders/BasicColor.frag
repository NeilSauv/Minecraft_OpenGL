#version 430

in flat vec3 vertexColor;
in flat vec3 vertexNormal; // La normale parfaite arrive d'ici

out vec4 FragColor;

const vec3 lightDir = normalize(vec3(0.5, 1.0, 0.3));
const vec3 ambientLight = vec3(0.35);

void main()
{
    // C'est magique : on normalise simplement la face !
    vec3 faceNormal = normalize(vertexNormal);
    
    float diff = max(dot(faceNormal, lightDir), 0.0);
    vec3 lighting = ambientLight + (diff * vec3(0.65));

    FragColor = vec4(vertexColor * lighting, 1.0);
}