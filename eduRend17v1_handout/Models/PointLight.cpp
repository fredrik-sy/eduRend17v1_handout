#include "PointLight.h"
#include "Common/Direct3D.h"

PointLight::PointLight(ID3D11Device* pDevice, IDXGISwapChain* pSwapChain, Window* pWindow)
{
	ID3DBlob* pCode;
	D3D11_INPUT_ELEMENT_DESC InputElementDescs[] = {
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEX",		0, DXGI_FORMAT_R32G32_FLOAT,	0, 48,	D3D11_INPUT_PER_VERTEX_DATA, 0 } };

	CompileShader(L"../assets/shaders/DrawTri.vs.hlsl", "VS_main", "vs_5_0", &pCode);
	CreateVertexShader(pDevice, pCode, &m_pVertexShader);
	CreateInputLayout(pDevice, InputElementDescs, ARRAYSIZE(InputElementDescs), pCode, &m_pInputLayout);
	SAFE_RELEASE(pCode);

	CompileShader(L"../assets/shaders/DrawTri.ps.hlsl", "PS_main", "ps_5_0", &pCode);
	CreatePixelShader(pDevice, pCode, &m_pPixelShader);
	SAFE_RELEASE(pCode);

	CreateDepthStencilResource(pDevice, pWindow->GetClientWidth(), pWindow->GetClientHeight(), &m_pDepthStencilResource);
	CreateDepthStencilView(pDevice, m_pDepthStencilResource, &m_pDepthStencilView);

	CreateShaderResource(pDevice, pWindow->GetClientWidth(), pWindow->GetClientHeight(), &m_pShaderResource);
	CreateShaderResourceView(pDevice, m_pShaderResource, &m_pShaderResourceView);
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
