#pragma once

#include <DirectXMath.h>
#include "source/vec/vec.h"

using namespace DirectX;
using namespace linalg;


struct MatrixBuffer
{
	XMMATRIX ModelToWorld;
	XMMATRIX WorldToView;
	XMMATRIX Projection;
};


struct MatrixBufferA
{
	mat4f ModelToWorld;
	mat4f WorldToView;
	mat4f Projection;
};