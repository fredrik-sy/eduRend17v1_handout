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

		m_PhongData.KaConstant = Material.KaConstant;
		m_PhongData.KdConstant = Material.KdConstant;
		m_PhongData.KsConstant = Material.KsConstant;
		m_PhongData.Shininess = 10.0;
		MapUpdateAndUnmapSubresource(pDeviceContext, m_pPhongBuffer, &m_PhongData, sizeof(PhongBuffer));

		pDeviceContext->PSSetShaderResources(0, 1, &Material.map_Ka_TexSRV);
		pDeviceContext->PSSetShaderResources(1, 1, &Material.map_Kd_TexSRV);
		pDeviceContext->PSSetShaderResources(2, 1, &Material.map_Ks_TexSRV);
		pDeviceContext->PSSetShaderResources(3, 1, &Material.map_d_TexSRV);
		pDeviceContext->PSSetShaderResources(4, 1, &Material.map_bump_TexSRV);
		pDeviceContext->DrawIndexed(IndexRange.size, IndexRange.start, 0);
	}
}
