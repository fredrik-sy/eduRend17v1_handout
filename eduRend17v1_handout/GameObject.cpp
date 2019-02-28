#include "Models/GameObject.h"
#include "Common/Direct3D.h"


GameObject::GameObject()
{
}


GameObject::GameObject(const char* Filename, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11Buffer* pPhongBuffer)
{
	mesh_t Mesh;
	Mesh.load_obj(Filename);

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
	SAFE_RELEASE(m_pPhongBuffer);
}
