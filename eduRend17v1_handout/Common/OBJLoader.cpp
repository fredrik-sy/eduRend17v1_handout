#include "OBJLoader.h"
#include "Common/Direct3DMath.h"

void OBJLoader::ReadData(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, mesh_t* pMesh, std::vector<unsigned int>* pIndices, std::vector<index_range_t>* pIndexRanges, std::vector<material_t>* pMaterials)
{
	size_t i_ofs = 0;

	for (drawcall_t& drawcall : pMesh->drawcalls)
	{
		for (triangle_t& tri : drawcall.tris)
		{
			pIndices->insert(pIndices->end(), tri.vi, tri.vi + 3);

			ComputeTangentSpace(
				pMesh->vertices[tri.vi[0]],
				pMesh->vertices[tri.vi[1]],
				pMesh->vertices[tri.vi[2]]);
		}

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
			wstr = std::wstring(material.map_bump.begin(), material.map_bump.end());
			DirectX::CreateWICTextureFromFile(pDevice, pDeviceContext, wstr.c_str(), &material.map_bump_Tex, &material.map_bump_TexSRV);
		}
	}
}


void OBJLoader::ComputeTangentSpace(vertex_t& V0, vertex_t& V1, vertex_t& V2)
{
	vec3f Q1 = V1.Pos - V0.Pos;
	vec3f Q2 = V2.Pos - V0.Pos;

	vec2f UV1 = V1.TexCoord - V0.TexCoord;
	vec2f UV2 = V2.TexCoord - V0.TexCoord;

	float R = 1.0f / (UV1.x * UV2.y - UV2.x * UV1.y);
	vec3f T = (Q1 * UV2.y - Q2 * UV1.y) * R;
	vec3f B = (Q2 * UV1.x - Q1 * UV2.x) * R;

	V0.Tangent = V1.Tangent = V2.Tangent = T.normalize();
	V0.Binormal = V1.Binormal = V2.Binormal = B.normalize();

	// Gram-Schmidt Orthogonalization.
	//V0.Tangent = (T - V0.Normal * V0.Normal.dot(T));
	//V1.Tangent = (T - V1.Normal * V1.Normal.dot(T));
	//V2.Tangent = (T - V2.Normal * V2.Normal.dot(T));

	//V0.Binormal = (B - V0.Normal * V0.Normal.dot(B) - V0.Tangent * V0.Tangent.dot(B)).normalize();
	//V1.Binormal = (B - V1.Normal * V1.Normal.dot(B) - V1.Tangent * V1.Tangent.dot(B)).normalize();
	//V2.Binormal = (B - V2.Normal * V2.Normal.dot(B) - V2.Tangent * V2.Tangent.dot(B)).normalize();

	//V0.Tangent = V0.Tangent.normalize();
	//V1.Tangent = V1.Tangent.normalize();
	//V2.Tangent = V2.Tangent.normalize();

	//if (CrossProduct(V0.Normal, V0.Tangent).dot(V0.Binormal) < 0.0f)
	//	V0.Tangent *= -1.0f;

	//if (CrossProduct(V1.Normal, V1.Tangent).dot(V1.Binormal) < 0.0f)
	//	V1.Tangent *= -1.0f;

	//if (CrossProduct(V2.Normal, V2.Tangent).dot(V2.Binormal) < 0.0f)
	//	V2.Tangent *= -1.0f;
}

