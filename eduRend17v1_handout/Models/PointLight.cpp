#include "PointLight.h"
#include "Common/Direct3D.h"

PointLight::PointLight(ID3D11Device* pDevice, IDXGISwapChain* pSwapChain, Window* pWindow)
{
	CreateRenderTargetView(pDevice, pSwapChain, &m_pRenderTargetView);
	CreateDepthStencilResource(pDevice, pWindow->GetClientWidth(), pWindow->GetClientHeight(), &m_pDepthStencilResource);
	CreateDepthStencilView(pDevice, m_pDepthStencilResource, &m_pDepthStencilView);

}


PointLight::~PointLight()
{
}


void PointLight::Update(float DeltaTime)
{
	mat4f rotation;

	rotation = mat4f::rotation(0, 0, 0);
	rotation.transpose();
	m_WorldToViewMatrices[0] = rotation * mat4f::translation(-m_Position);

	rotation = mat4f::rotation(0, fPI / 2.0f, 0);
	rotation.transpose();
	m_WorldToViewMatrices[1] = rotation * mat4f::translation(-m_Position);

	rotation = mat4f::rotation(0, fPI, 0);
	rotation.transpose();
	m_WorldToViewMatrices[2] = rotation * mat4f::translation(-m_Position);

	rotation = mat4f::rotation(0, (3.0f * fPI) / 2.0f, 0);
	rotation.transpose();
	m_WorldToViewMatrices[3] = rotation * mat4f::translation(-m_Position);

	rotation = mat4f::rotation(0, 0, fPI / 2.0f);
	rotation.transpose();
	m_WorldToViewMatrices[4] = rotation * mat4f::translation(-m_Position);

	rotation = mat4f::rotation(0, 0, -fPI / 2.0f);
	rotation.transpose();
	m_WorldToViewMatrices[5] = rotation * mat4f::translation(-m_Position);
}


void PointLight::Render(ID3D11DeviceContext * pDeviceContext)
{
}

inline mat4f PointLight::GetWorldToViewMatrix()
{
	return mat4f();
}

inline mat4f PointLight::GetProjectionMatrix()
{
	return mat4f();
}
