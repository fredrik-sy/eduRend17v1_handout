#pragma once

#include "source/vec/mat.h"

using namespace linalg;


struct LightMatrixBuffer
{
	mat4f WorldToView;
	mat4f Projection;
};