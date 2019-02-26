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

	m_Transformation = XMMatrixTranslation(0.0f, -5.0f, 0.0f)
		* XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), (float)M_PI / 2)
		* XMMatrixScaling(0.05f, 0.05f, 0.05f);

	m_TransformationA = mat4f::translation(0.0f, -1.0f, -10.0f)
		* mat4f::scaling(1.0f);

	CreateConstantBuffer(pDevice, sizeof(MaterialBufferA), &m_pMaterialBuffer);
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
	UINT32 stride = sizeof(vertex_t);
	UINT32 offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pDeviceContext->PSSetConstantBuffers(2, 1, &m_pMaterialBuffer);

	for (auto& irange : m_IndexRanges)
	{
		// Fetch material
		const material_t& mtl = m_Materials[irange.mtl_index];

		m_MaterialDataA.Ka = mtl.Ka;
		m_MaterialDataA.Kd = mtl.Kd;
		m_MaterialDataA.Ks = mtl.Ks;
		m_MaterialDataA.Shininess = 10.0;
		MapUpdateAndUnmapSubresource(pDeviceContext, m_pMaterialBuffer, &m_MaterialDataA, sizeof(MaterialBufferA));

		// Bind textures
		pDeviceContext->PSSetShaderResources(0, 1, &mtl.map_Ka_TexSRV);
		pDeviceContext->PSSetShaderResources(1, 1, &mtl.map_Kd_TexSRV);
		pDeviceContext->PSSetShaderResources(2, 1, &mtl.map_Ks_TexSRV);
		pDeviceContext->PSSetShaderResources(3, 1, &mtl.map_d_TexSRV);
		pDeviceContext->PSSetShaderResources(4, 1, &mtl.map_bump_TexSRV);

		// Make the drawcall
		pDeviceContext->DrawIndexed(irange.size, irange.start, 0);
	}
}
