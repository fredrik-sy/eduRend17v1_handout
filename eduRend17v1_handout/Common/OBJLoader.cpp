#include "OBJLoader.h"

#include "source/mesh.h"
#include "source/drawcall.h"

OBJLoader::OBJLoader()
{
}


OBJLoader::~OBJLoader()
{
}

void OBJLoader::ReadData(const char* pFilename, std::vector<unsigned int>* pIndices, std::vector<index_range_t>* pIndexRanges)
{
	mesh_t* mesh = new mesh_t();
	mesh->load_obj(pFilename);

	size_t i_ofs = 0;

	for (auto& dc : mesh->drawcalls)
	{
		// Append the drawcall indices
		for (auto& tri : dc.tris)
			pIndices->insert(pIndices->end(), tri.vi, tri.vi + 3);

		// Create a range
		size_t i_size = dc.tris.size() * 3;
		int mtl_index = dc.mtl_index > -1 ? dc.mtl_index : -1;
		pIndexRanges->push_back({ i_ofs, i_size, 0, mtl_index });

		i_ofs = pIndices->size();
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

}

void OBJLoader::ParseOBJ()
{

}

void OBJLoader::ParseMTL()
{
}
