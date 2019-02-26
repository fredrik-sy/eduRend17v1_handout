#include "OBJLoader.h"


void OBJLoader::ReadData(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, mesh_t* pMesh, std::vector<unsigned int>* pIndices, std::vector<index_range_t>* pIndexRanges, std::vector<material_t>* pMaterials)
{
	size_t i_ofs = 0;

	for (drawcall_t& drawcall : pMesh->drawcalls)
	{
		for (triangle_t& tri : drawcall.tris)
			pIndices->insert(pIndices->end(), tri.vi, tri.vi + 3);								// The range includes all except the last.

		size_t i_size = drawcall.tris.size() * 3;
		int mtl_index = drawcall.mtl_index > -1 ? drawcall.mtl_index : -1;
		pIndexRanges->push_back({ i_ofs, i_size, 0, mtl_index });

		i_ofs = pIndices->size();
	}

	pMaterials->insert(pMaterials->end(), pMesh->materials.begin(), pMesh->materials.end());

	for (material_t& material : *pMaterials)
	{
		std::wstring wstr;

		if (material.map_Ka.size())
		{
			wstr = std::wstring(material.map_Ka.begin(), material.map_Ka.end());
			DirectX::CreateWICTextureFromFile(pDevice, pDeviceContext, wstr.c_str(), &material.map_Ka_Tex, &material.map_Ka_TexSRV);
		}

		if (material.map_Kd.size())
		{
			wstr = std::wstring(material.map_Kd.begin(), material.map_Kd.end());
			DirectX::CreateWICTextureFromFile(pDevice, pDeviceContext, wstr.c_str(), &material.map_Kd_Tex, &material.map_Kd_TexSRV);
		}

		if (material.map_Ks.size())
		{
			wstr = std::wstring(material.map_Ks.begin(), material.map_Ks.end());
			DirectX::CreateWICTextureFromFile(pDevice, pDeviceContext, wstr.c_str(), &material.map_Ks_Tex, &material.map_Ks_TexSRV);
		}

		if (material.map_d.size())
		{
			wstr = std::wstring(material.map_d.begin(), material.map_d.end());
			DirectX::CreateWICTextureFromFile(pDevice, pDeviceContext, wstr.c_str(), &material.map_d_Tex, &material.map_d_TexSRV);
		}

		if (material.map_bump.size())
		{
			wstr = std::wstring(material.map_Kd.begin(), material.map_Kd.end());
			DirectX::CreateWICTextureFromFile(pDevice, pDeviceContext, wstr.c_str(), &material.map_bump_Tex, &material.map_bump_TexSRV);
		}
	}
}

