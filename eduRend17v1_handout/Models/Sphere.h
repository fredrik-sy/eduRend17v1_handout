#pragma once

#include "GameObject.h"
#include "Common/OBJLoader.h"
#include <DirectXMath.h>

class Sphere : public GameObject
{
public:
	Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~Sphere();

	virtual void Update() override;
	virtual void Render(ID3D11DeviceContext * pDeviceContext) override;
	virtual inline mat4f GetTransformationMatrix() override;

private:
	std::vector<index_range_t> m_IndexRanges;
	std::vector<material_t> m_Materials;

	mat4f m_Transformation;
	MaterialBuffer m_MaterialData;

};


inline mat4f Sphere::GetTransformationMatrix()
{
	return m_Transformation;
}

