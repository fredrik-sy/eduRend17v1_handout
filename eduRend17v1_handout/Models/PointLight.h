#pragma once

#include "Models/GameObject.h"

class PointLight : public GameObject
{
public:
	PointLight(ID3D11Device* pDevice);
	virtual ~PointLight();

	virtual void Update(float DeltaTime) override;
	virtual void Render(ID3D11DeviceContext * pDeviceContext) override;

	virtual inline mat4f GetTransformationMatrix() override;
	virtual inline void SetPosition(float x, float y, float z);
	virtual inline vec3f GetPosition();

	virtual mat4f GetWorldToViewMatrix();
	virtual mat4f GetProjectionMatrix();

private:
	ID3D11Texture2D* m_DepthMaps[6];
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11Texture2D* m_pDepthStencilResource;

	vec3f m_Position;
	mat4f m_WorldToViewMatrices[6];

};


inline mat4f PointLight::GetTransformationMatrix()
{
	return mat4f::translation(m_Position);
}


inline void PointLight::SetPosition(float x, float y, float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}


inline vec3f PointLight::GetPosition()
{
	return m_Position;
}
