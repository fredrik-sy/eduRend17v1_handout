#pragma once

#include "source/vec/vec.h"

using namespace linalg;

struct PositionBuffer
{
	vec3f LightPosition;
	float Padding3;
	vec3f CameraPosition;
	float Padding4;
};