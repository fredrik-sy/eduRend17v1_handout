#pragma once


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
	OBJLoader();
	virtual ~OBJLoader();
	void ReadData(const char* pFilename, std::vector<unsigned int>* pIndices, std::vector<index_range_t>* pIndexRanges);

private:
	void ParseOBJ();
	void ParseMTL();

};

