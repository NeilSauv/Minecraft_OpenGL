#version 330 core
out vec4 FragColor;

in vec4 TexCoord;


// texture sampler
uniform sampler2D texture1;

void main()
{
	FragColor = texture2D(texture1, TexCoord.xy);
}  

