#include "WoodDoll.h"

#include "Common/Direct3D.h"
#include "source/mesh.h"
#include "Common/OBJLoader.h"

WoodDoll::WoodDoll(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11Buffer* pPhongBuffer)
	: GameObject("../assets/wooddoll/wooddoll.obj", pDevice, pDeviceContext, pPhongBuffer)
{
}


WoodDoll::~WoodDoll()
{
}


void WoodDoll::Update(float DeltaTime)
{
	m_AngleY += (fPI / 2) * DeltaTime;

	if (m_AngleY / (2 * fPI) >= 1)
		m_AngleY -= (2 * fPI);
}


void WoodDoll::Render(ID3D11DeviceContext * pDeviceContext)
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
		m_PhongData.Shininess = 10.0f;
		MapUpdateAndUnmapSubresource(pDeviceContext, m_pPhongBuffer, &m_PhongData, sizeof(PhongBuffer));

		UINT StartSlot = 0;

		if (Material.map_Kd_TexSRV)
			pDeviceContext->PSSetShaderResources(StartSlot++, 1, &Material.map_Kd_TexSRV);

		pDeviceContext->DrawIndexed(IndexRange.size, IndexRange.start, 0);

		// Clears all shaders to avoid being used by other models.
		for (ID3D11ShaderResourceView* pShaderResourceViews[1] = { nullptr }; StartSlot > 0; StartSlot--)
			pDeviceContext->PSSetShaderResources(StartSlot - 1, 1, pShaderResourceViews);
	}
}
