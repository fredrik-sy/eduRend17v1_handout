#pragma once

#include <d3d11.h>
#include "source/drawcall.h"
#include "source/mesh.h"


struct index_range_t
{
	size_t start;
	size_t size;
	unsigned ofs;
	int mtl_index;
};


class OBJLoader
{
public:
	static void ReadData(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, mesh_t* pMesh, std::vector<unsigned int>* pIndices, std::vector<index_range_t>* pIndexRanges, std::vector<material_t>* pMaterials);

private:
	static void ComputeTangentSpace(vertex_t& V0, vertex_t& V1, vertex_t& V2);

};

