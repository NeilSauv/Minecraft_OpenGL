#version 430

// AJOUT DU MOT CLÉ "flat" ICI AUSSI
in flat vec3 vertexColor;
in vec3 FragPos;

out vec4 FragColor;

const vec3 lightDir = normalize(vec3(0.5, 1.0, 0.3));
const vec3 ambientLight = vec3(0.35);

void main()
{
    vec3 xTangent = dFdx(FragPos);
    vec3 yTangent = dFdy(FragPos);
    vec3 faceNormal = normalize(cross(xTangent, yTangent));
    
    float diff = max(dot(faceNormal, lightDir), 0.0);
    vec3 lighting = ambientLight + (diff * vec3(0.65));

    FragColor = vec4(vertexColor * lighting, 1.0);
}