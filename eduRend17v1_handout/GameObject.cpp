#include "Models/GameObject.h"
#include "Common/Direct3D.h"


GameObject::GameObject()
{
}


GameObject::GameObject(const char* pFilename, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11Buffer* pPhongBuffer)
{
	mesh_t Mesh;
	Mesh.load_obj(pFilename);

	std::vector<unsigned int> Indices;

	OBJLoader::ReadData(pDevice, pDeviceContext, &Mesh, &Indices, &m_IndexRanges, &m_Materials);

	CreateVertexBuffer(pDevice, &Mesh.vertices, Mesh.vertices.size() * sizeof(vertex_t), &m_pVertexBuffer);
	CreateIndexBuffer(pDevice, &Indices, &m_pIndexBuffer);
	m_pPhongBuffer = pPhongBuffer;
}


GameObject::~GameObject()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);

	for (material_t& Material : m_Materials)
	{
		SAFE_RELEASE(Material.map_Ka_Tex);
		SAFE_RELEASE(Material.map_Ka_TexSRV);

		SAFE_RELEASE(Material.map_Kd_Tex);
		SAFE_RELEASE(Material.map_Kd_TexSRV);

		SAFE_RELEASE(Material.map_Ks_Tex);
		SAFE_RELEASE(Material.map_Ks_TexSRV);

		SAFE_RELEASE(Material.map_d_Tex);
		SAFE_RELEASE(Material.map_d_TexSRV);

		SAFE_RELEASE(Material.map_bump_Tex);
		SAFE_RELEASE(Material.map_bump_TexSRV);
	}
}
