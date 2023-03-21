 #include <cglm/cglm.h>
#include <math.h>

#include "../main.h"

#include "../Generators/Chunk/Headers/ChunkHeaders.h"
#include "../Generators/Noises/Headers/NoisesHeaders.h"
#include "../Utils/Headers/UtilsHeaders.h"
#include "../Textures/Headers/TextureHeaders.h"
#include "../Physics/Headers/PhysicsHeaders.h"

int RoundPos(float pos);
float RoundPosf(float pos);
bool SameBlock(int actual[], int last[]);

void mat4_vec4_mul(mat4 m1, vec4 m2, vec4 dest);

vec3 currentRay = GLM_VEC3_ZERO_INIT;


bool RayCast(vec3 pos, vec3 forward, float distance, float increaseValue, vec3 dest)
{
	vec3 position = {pos[0], pos[1], pos[2]};
	position[0] += 0.5f;
	position[1] += 0.5f;
	position[2] += 0.5f;
	vec3 forwardDir = { forward[0], forward[1], forward[2] };
	vec3 increaseVec = { increaseValue, increaseValue, increaseValue };

	int lastBlock[] = { RoundPos(position[0]), RoundPos(position[1]), RoundPos(position[2]) };

	float actualDistance = 0.0f;

	while (actualDistance < distance)
	{
		float height = GetSingleNoiseVal(position[0], position[2], NULL, heightNoise) * ChunkHeight/2;

		int actualBlock[] = {RoundPos(position[0]), RoundPos(position[1]), RoundPos(position[2])};
		dest[0] = RoundPos(position[0]);
		dest[1] = RoundPos(position[1]);
		dest[2] = RoundPos(position[2]);

		if (height + 1 >= position[1] && !SameBlock(actualBlock, lastBlock) && DestroyPossible(actualBlock))
		{
			for (int i = 0; i < 3; i++)
			{
				dest[i] = actualBlock[i];
			}

			return true;
		}

		for (int i = 0; i < 3; i++)
		{
			lastBlock[i] = actualBlock[i];
		}
		actualDistance += increaseValue;
		glm_vec3_muladd(increaseVec, forwardDir, position);

	}

	return false;
}

bool SameBlock(int actual[], int last[])
{
	for (int i = 0; i < 3; i++)
	{
		if (actual[i] != last[i])
			return false;
	}
	return true;
}

int RoundPos(float pos)
{
	if (pos < 0)
		return (int)pos - 1;

	return (int)pos ;
}

float RoundPosf(float pos)
{
	if (pos < 0)
		return (float) ((int) pos - 1);

	return (float)((int) pos);
}

void mat4_vec4_mul(mat4 m1, vec4 m2, vec4 dest)
{
	float a00 = m1[0][0], a01 = m1[0][1], a02 = m1[0][2], a03 = m1[0][3],
		  a10 = m1[1][0], a11 = m1[1][1], a12 = m1[1][2], a13 = m1[1][3],
		  a20 = m1[2][0], a21 = m1[2][1], a22 = m1[2][2], a23 = m1[2][3],
		  a30 = m1[3][0], a31 = m1[3][1], a32 = m1[3][2], a33 = m1[3][3];

	float b00 = m2[0], b01 = m2[1], b02 = m2[2], b03 = m2[3];
	
	dest[0] = a00 * b00 + a10 * b01 + a20 * b02 + a30 * b03;
	dest[1] = a01 * b00 + a11 * b01 + a21 * b02 + a31 * b03;
	dest[2] = a02 * b00 + a12 * b01 + a22 * b02 + a32 * b03;
	dest[3] = a03 * b00 + a13 * b01 + a23 * b02 + a33 * b03;
}