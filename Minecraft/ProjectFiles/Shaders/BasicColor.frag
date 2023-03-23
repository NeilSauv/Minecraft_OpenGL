#version 430
out vec4 FragColor;

in vec2 TexCoord;


// texture sampler
uniform sampler2D texture1;

void main()
{
	FragColor = texture2D(texture1, TexCoord);
}  

