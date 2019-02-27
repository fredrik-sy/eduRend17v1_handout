#pragma once

#include "Models/GameObject.h"
#include "Common/Direct3D.h"
#include "source/vec/mat.h"

class WoodDoll : public GameObject
{
public:
	WoodDoll(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~WoodDoll();

	virtual void Update(float DeltaTime) override;
	virtual void Render(ID3D11DeviceContext * pDeviceContext) override;
	virtual inline mat4f GetTransformationMatrix() override;

private:
	float m_AngleY;

};


inline mat4f WoodDoll::GetTransformationMatrix()
{
	return mat4f::translation(0.0f, -5.30f, 25.0f)
		* mat4f::rotation(0.0f, m_AngleY, 0.0f)
		* mat4f::scaling(10.0f);
}
