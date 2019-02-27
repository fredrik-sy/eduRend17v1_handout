#pragma once

#include "source/drawcall.h"
#include "Buffers/MaterialBuffer.h"
#include "Common/OBJLoader.h"
#include "source/vec/mat.h"
#include <d3d11.h>
#include <vector>

class GameObject
{
public:
	GameObject();
	GameObject(const char* Filename, ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual ~GameObject();
	virtual void Update(float DeltaTime) = 0;
	virtual void Render(ID3D11DeviceContext* pDeviceContext) = 0;
	virtual inline mat4f GetTransformationMatrix() = 0;
	
protected:
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	ID3D11Buffer* m_pMaterialBuffer;

	std::vector<index_range_t> m_IndexRanges;
	std::vector<material_t> m_Materials;
	MaterialBuffer m_MaterialData;
};

