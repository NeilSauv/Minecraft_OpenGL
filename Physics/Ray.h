#ifndef RAY
#define RAY

#include <cglm/cglm.h>

extern vec3 currentRay;

void UpdateRay(mat4 viewMatrix, mat4 projectionMatrix);

bool RayCast(vec3 position, vec3 direction, float distance, float increaseValue,
             vec3 dest);

#endif // !RAY
