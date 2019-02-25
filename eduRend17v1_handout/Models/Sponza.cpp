#include "Sponza.h"
#include "Common/Direct3D.h"
#include "source/mesh.h"

Sponza::Sponza(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	mesh_t* mesh = new mesh_t();
	mesh->load_obj("../assets/crytek-sponza/sponza.obj");

	// Load and organize indices in ranges per drawcall (material)
	std::vector<unsigned> indices;
	size_t i_ofs = 0;

	for (auto& dc : mesh->drawcalls)
	{
		// Append the drawcall indices
		for (auto& tri : dc.tris)
			indices.insert(indices.end(), tri.vi, tri.vi + 3);

		// Create a range
		size_t i_size = dc.tris.size() * 3;
		int mtl_index = dc.mtl_index > -1 ? dc.mtl_index : -1;
		m_IndexRanges.push_back({ i_ofs, i_size, 0, mtl_index });

		i_ofs = indices.size();
	}

	CreateVertexBuffer(pDevice, &mesh->vertices, mesh->vertices.size() * sizeof(vertex_t), &m_pVertexBuffer);
	CreateIndexBuffer(pDevice, &indices, &m_pIndexBuffer);

	// Copy materials from mesh
	m_Materials.insert(m_Materials.end(), mesh->materials.begin(), mesh->materials.end());

	// Go through materials and load textures (if any) to device

	for (auto& mtl : m_Materials)
	{
		HRESULT hr;
		std::wstring wstr; // for conversion from string to wstring

		// map_Kd (diffuse texture)
		if (mtl.map_Kd.size()) {
			// Convert the file path string to wstring
			wstr = std::wstring(mtl.map_Kd.begin(), mtl.map_Kd.end());
			// Load texture to device and obtain pointers to it
			hr = DirectX::CreateWICTextureFromFile(pDevice, pDeviceContext, wstr.c_str(), &mtl.map_Kd_Tex, &mtl.map_Kd_TexSRV);
			// Say how it went
			printf("loading texture %s - %s\n", mtl.map_Kd.c_str(), SUCCEEDED(hr) ? "OK" : "FAILED");
		}
		
		// Same thing with other textres here such as mtl.map_bump (Bump/Normal texture) etc
		//
		// ...
	}

	SAFE_DELETE(mesh);

	m_Transformation = XMMatrixTranslation(0.0f, -5.0f, 0.0f)
		* XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), (float)M_PI / 2)
		* XMMatrixScaling(0.05f, 0.05f, 0.05f);

	m_TransformationA = mat4f::translation(0.0f, -5.0f, 0.0f)
		* mat4f::rotation(fPI / 2, 0.0f, 1.0f, 0.0f)
		* mat4f::scaling(0.05f);

	m_MaterialDataA.Ka = vec3f(0.0f, 0.5880f, 1.0f);
	m_MaterialDataA.Kd = vec3f(0.5880f, 0.5880f, 0.5880f);
	m_MaterialDataA.Ks = vec3f(10.0f, 10.0f, 10.0f);
	m_MaterialDataA.Shininess = 10.0f;
}


Sponza::~Sponza()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
}

void Sponza::Update()
{
}

void Sponza::Render(ID3D11DeviceContext* pDeviceContext)
{
	UINT32 stride = sizeof(vertex_t);
	UINT32 offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Iterate drawcalls
	for (auto& irange : m_IndexRanges)
	{
		// Fetch material
		const material_t& mtl = m_Materials[irange.mtl_index];

		// Bind textures
		pDeviceContext->PSSetShaderResources(0, 1, &mtl.map_Kd_TexSRV);
		// ...other textures here (see material_t)

		// Make the drawcall
		pDeviceContext->DrawIndexed(irange.size, irange.start, 0);
	}
}
