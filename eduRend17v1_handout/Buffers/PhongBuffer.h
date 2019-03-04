#pragma once

#include "source/vec/vec.h"

using namespace linalg;


struct PhongBuffer
{
	vec3f KaConstant;					// Ambient reflection constant
	float Padding3;
	vec3f KdConstant;					// Diffuse reflection constant
	float Padding4;
	vec3f KsConstant;					// Specular reflection constant
	float Shininess;			// Material shininess constant
};