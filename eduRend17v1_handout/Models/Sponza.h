#pragma once

#include "Models/GameObject.h"
#include "Common/OBJLoader.h"

using namespace DirectX;

class Sponza : public GameObject
{
public:
	Sponza(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11Buffer* pPhongBuffer);
	virtual ~Sponza();

	virtual void Update(float DeltaTime) override;
	virtual void Render(ID3D11DeviceContext* pDeviceContext) override;
	virtual inline mat4f GetTransformationMatrix() override;

};


inline mat4f Sponza::GetTransformationMatrix()
{
	return mat4f::translation(0.0f, -5.0f, 0.0f)
		* mat4f::rotation(fPI / 2, 0.0f, 1.0f, 0.0f)
		* mat4f::scaling(0.05f);
}

