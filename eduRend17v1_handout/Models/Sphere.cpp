#include "Sphere.h"
#include "Common/Direct3D.h"
#include "source/mesh.h"


Sphere::Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11Buffer* pPhongBuffer)
	: GameObject("../assets/sphere/sphere.obj", pDevice, pDeviceContext, pPhongBuffer)
{
}


Sphere::~Sphere()
{
}


void Sphere::Update(float DeltaTime)
{
}


void Sphere::Render(ID3D11DeviceContext * pDeviceContext)
{
	UINT32 Strides = sizeof(vertex_t);
	UINT32 Offsets = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &Strides, &Offsets);
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	for (auto& IndexRange : m_IndexRanges)
	{
		const material_t& Material = m_Materials[IndexRange.mtl_index];

		m_PhongData.Ka = Material.Ka;
		m_PhongData.Kd = Material.Kd;
		m_PhongData.Ks = Material.Ks;
		m_PhongData.Shininess = 10.0;
		MapUpdateAndUnmapSubresource(pDeviceContext, m_pPhongBuffer, &m_PhongData, sizeof(PhongBuffer));

		pDeviceContext->DrawIndexed(IndexRange.size, IndexRange.start, 0);
	}
}
