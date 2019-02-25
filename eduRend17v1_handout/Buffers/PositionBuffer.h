#pragma once

#include "source/vec/vec.h"
#include <DirectXMath.h>

using namespace DirectX;
using namespace linalg;

struct PositionBuffer
{
	XMVECTOR LightPosition;
	XMVECTOR CameraPosition;
};


struct PositionBufferA
{
	vec3f LightPosition;
	float Padding3;
	vec3f CameraPosition;
	float Padding4;
};