#pragma once

#include "Models/GameObject.h"
#include <DirectXMath.h>

using namespace DirectX;

class Sponza : public GameObject
{
public:
	Sponza(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~Sponza();

	virtual void Update() override;
	virtual void Render(ID3D11DeviceContext* pDeviceContext) override;
	virtual inline XMMATRIX GetTransformationMatrix() override;

private:
	XMMATRIX m_Transformation;
	std::vector<index_range_t> m_IndexRanges;
	std::vector<material_t> m_Materials;


	////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	virtual inline mat4f GetTransformationMatrixA() override;

private:
	mat4f m_TransformationA;
	MaterialBufferA m_MaterialDataA;

};


inline XMMATRIX Sponza::GetTransformationMatrix()
{
	return m_Transformation;
}


////////////////////////////////////////////////////////////////////////////////////////////////////


inline mat4f Sponza::GetTransformationMatrixA()
{
	return m_TransformationA;
}

