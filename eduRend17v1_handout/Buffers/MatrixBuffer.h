#pragma once

#include "source/vec/vec.h"

using namespace linalg;


struct MatrixBuffer
{
	mat4f ModelToWorld;
	mat4f WorldToView;
	mat4f Projection;
};