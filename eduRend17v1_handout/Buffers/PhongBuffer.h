#pragma once

#include "source/vec/vec.h"

using namespace linalg;


struct PhongBuffer
{
	vec3f Ka;					// Ambient reflection constant
	float Padding3;
	vec3f Kd;					// Diffuse reflection constant
	float Padding4;
	vec3f Ks;					// Specular reflection constant
	float Shininess;			// Material shininess constant
};