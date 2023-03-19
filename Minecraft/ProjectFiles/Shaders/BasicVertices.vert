#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in float aFace;
layout (location = 3) in vec3 aOffset;
layout (location = 4) in vec4 aRender;

out vec4 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	int single = aRender.z > 0 ? 0 : 1;

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



	gl_Position =  projection * view * model * vec4(pos + aOffset, 1.0);
	float u = (aRender.y + aFace * single)/25.0 + aTexCoord.x /25.0;
	float v = aRender.z/25.0 + aTexCoord.y /25.0;

	TexCoord = vec4(u,v, aTexCoord.xy);
}