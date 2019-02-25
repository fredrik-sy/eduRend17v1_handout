#pragma once

#include "source/vec/vec.h"
#include <DirectXMath.h>

using namespace DirectX;
using namespace linalg;


struct MaterialBuffer
{
	XMVECTOR Ks;				// Specular reflection constant
	XMVECTOR Kd;				// Diffuse reflection constant
	XMFLOAT3 Ka;				// Ambient reflection constant
	float shininess;			// Material shininess constant
};


struct MaterialBufferA
{
	vec3f Ka;					// Ambient reflection constant
	float Padding3;
	vec3f Kd;					// Diffuse reflection constant
	float Padding4;
	vec3f Ks;					// Specular reflection constant
	float Shininess;			// Material shininess constant
};