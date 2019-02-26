#pragma once

#include "source/drawcall.h"
#include "source/vec/mat.h"
#include "Buffers/MaterialBuffer.h"
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;


class GameObject
{
public:
	GameObject() {};
	virtual ~GameObject() {};
	virtual void Update() = 0;
	virtual void Render(ID3D11DeviceContext* pDeviceContext) = 0;
	virtual inline mat4f GetTransformationMatrix() = 0;
	
protected:
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	ID3D11Buffer* m_pMaterialBuffer;

};

