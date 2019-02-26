#include "Sphere.h"
#include "Common/Direct3D.h"
#include "source/mesh.h"


Sphere::Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	mesh_t Mesh;
	Mesh.load_obj("../assets/sphere/sphere.obj");

	std::vector<unsigned int> Indices;

	OBJLoader::ReadData(pDevice, pDeviceContext, &Mesh, &Indices, &m_IndexRanges, &m_Materials);

	CreateVertexBuffer(pDevice, &Mesh.vertices, Mesh.vertices.size() * sizeof(vertex_t), &m_pVertexBuffer);
	CreateIndexBuffer(pDevice, &Indices, &m_pIndexBuffer);
	CreateConstantBuffer(pDevice, sizeof(MaterialBuffer), &m_pMaterialBuffer);

	m_Transformation = mat4f::translation(0.0f, -1.0f, -10.0f)
		* mat4f::scaling(1.0f);
}


Sphere::~Sphere()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pMaterialBuffer);
}


void Sphere::Update()
{
}


void Sphere::Render(ID3D11DeviceContext * pDeviceContext)
{
	UINT32 Strides = sizeof(vertex_t);
	UINT32 Offsets = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &Strides, &Offsets);
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pDeviceContext->PSSetConstantBuffers(2, 1, &m_pMaterialBuffer);

	for (auto& IndexRange : m_IndexRanges)
	{
		const material_t& Material = m_Materials[IndexRange.mtl_index];

		m_MaterialData.Ka = Material.Ka;
		m_MaterialData.Kd = Material.Kd;
		m_MaterialData.Ks = Material.Ks;
		m_MaterialData.Shininess = 10.0;
		MapUpdateAndUnmapSubresource(pDeviceContext, m_pMaterialBuffer, &m_MaterialData, sizeof(MaterialBuffer));

		pDeviceContext->PSSetShaderResources(0, 1, &Material.map_Ka_TexSRV);
		pDeviceContext->PSSetShaderResources(1, 1, &Material.map_Kd_TexSRV);
		pDeviceContext->PSSetShaderResources(2, 1, &Material.map_Ks_TexSRV);
		pDeviceContext->PSSetShaderResources(3, 1, &Material.map_d_TexSRV);
		pDeviceContext->PSSetShaderResources(4, 1, &Material.map_bump_TexSRV);

		pDeviceContext->DrawIndexed(IndexRange.size, IndexRange.start, 0);
	}
}
