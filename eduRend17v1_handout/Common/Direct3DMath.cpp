#include "Direct3DMath.h"


vec3f CrossProduct(vec3f V1, vec3f V2)
{
	vec3f Result;
	Result.x = (V1.y * V2.z) - (V1.z * V2.y);
	Result.y = (V1.z * V2.x) - (V1.x * V2.z);
	Result.z = (V1.x * V2.y) - (V1.y * V2.x);
	return Result;
}
