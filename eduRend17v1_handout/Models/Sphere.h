#pragma once

#include "GameObject.h"
#include "Common/OBJLoader.h"
#include <DirectXMath.h>

class Sphere : public GameObject
{
public:
	Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11Buffer* pPhongBuffer);
	virtual ~Sphere();

	virtual void Update(float DeltaTime) override;
	virtual void Render(ID3D11DeviceContext * pDeviceContext) override;
	virtual inline mat4f GetTransformationMatrix() override;

};


inline mat4f Sphere::GetTransformationMatrix()
{
	return mat4f::translation(0.0f, -3.5f, -10.0f)
		* mat4f::scaling(1.0f);
}

