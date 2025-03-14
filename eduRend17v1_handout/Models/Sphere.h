#pragma once

#include "GameObject.h"

class Sphere : public GameObject
{
public:
	Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~Sphere();

	virtual void Update() override;
	virtual void Render(ID3D11DeviceContext * pDeviceContext) override;
	virtual inline XMMATRIX GetTransformationMatrix() override;

private:
	XMMATRIX m_Transformation;
	std::vector<index_range_t> m_IndexRanges;
	std::vector<material_t> m_Materials;


	////////////////////////////////////////////////////////////////////////////////////////////////////


public:
	virtual inline mat4f GetTransformationMatrixA() override;
	virtual inline MaterialBufferA GetMaterialDataA() override;

private:
	mat4f m_TransformationA;
	MaterialBufferA m_MaterialDataA;

};


inline XMMATRIX Sphere::GetTransformationMatrix()
{
	return XMMATRIX();
}


////////////////////////////////////////////////////////////////////////////////////////////////////


inline mat4f Sphere::GetTransformationMatrixA()
{
	return m_TransformationA;
}


inline MaterialBufferA Sphere::GetMaterialDataA()
{
	return m_MaterialDataA;
}
