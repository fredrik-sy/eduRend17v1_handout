#pragma once

#include <d3d11.h>
#include <vector>


//
//   PURPOSE: Creates a device that represents the display adapter and a swap chain used for rendering.
//
void CreateDeviceAndSwapChain(HWND OutputWindow, ID3D11Device** ppDevice, ID3D11DeviceContext** ppDeviceContext, IDXGISwapChain** ppSwapChain);

//
//   PURPOSE: Creates a render-target view for accessing resource data.
//
void CreateRenderTargetView(ID3D11Device* pDevice, IDXGISwapChain* pSwapChain, ID3D11RenderTargetView** ppRenderTargetView);
void CreateRenderTargetView(ID3D11Device* pDevice, IDXGISwapChain* pSwapChain, ID3D11Texture2D* pBuffer, ID3D11RenderTargetView** ppRenderTargetView);


//
//   PURPOSE: Binds a texture as a depth-stencil target.
//
void CreateDepthStencilBuffer(ID3D11Device* pDevice, UINT Width, UINT Height, ID3D11Texture2D** ppDepthStencilResource);


//
//   PURPOSE: Creates a depth-stencil view for accessing resource data.
//
void CreateDepthStencilView(ID3D11Device* pDevice, ID3D11Texture2D* pDepthStencilResource, ID3D11DepthStencilView** ppDepthStencilView);


//
//   PURPOSE: Creates a rasterizer state object that tells the rasterizer stage how to behave.
//
void CreateRasterizerState(ID3D11Device* pDevice, ID3D11RasterizerState** ppRasterizerState, D3D11_CULL_MODE CullMode);


//
//   PURPOSE: Creates a sampler-state object that encapsulates sampling information for a texture.
//
void CreateSamplerState(ID3D11Device* pDevice, ID3D11SamplerState** ppSamplerState);
void CreateComparisonSamplerState(ID3D11Device* pDevice, ID3D11SamplerState** ppSamplerState);


//
//   PURPOSE: Creates a buffer for vertex data.
//
void CreateVertexShader(ID3D11Device* pDevice, ID3DBlob* pCode, ID3D11VertexShader** ppVertexShader);


//
//   PURPOSE: Creates an input-layout object to describe the input-buffer data for the input-assembler stage.
//
void CreateInputLayout(ID3D11Device* pDevice, const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, UINT NumElements, ID3DBlob* pCode, ID3D11InputLayout** ppInputLayout);


//
//   PURPOSE: Creates a pixel-shader object from a compiled shader.
//
void CreatePixelShader(ID3D11Device* pDevice, ID3DBlob* pCode, ID3D11PixelShader** ppPixelShader);


//
//   PURPOSE: Compiles HLSL code into bytecode for a given target.
//
void CompileShader(LPCWSTR pFileName, LPCSTR pEntrypoint, LPCSTR pTarget, ID3DBlob** ppCode);


//
//   PURPOSE: Creates a shader-constant buffer.
//
void CreateConstantBuffer(ID3D11Device* pDevice, UINT ByteWidth, ID3D11Buffer** ppBuffer);


//
//   PURPOSE: Creates a buffer for index data.
//
void CreateIndexBuffer(ID3D11Device* pDevice, std::vector<unsigned int>* pIndices, ID3D11Buffer** ppBuffer);


//
//   PURPOSE: Copy data from memory to a subresource.
//
void MapUpdateAndUnmapSubresource(ID3D11DeviceContext* pDeviceContext, ID3D11Resource* pResource, void* pData, UINT Size);


//
//   PURPOSE: Binds a texture as a shader target.
//
void CreateShaderResourceBuffer(ID3D11Device* pDevice, UINT Width, UINT Height, ID3D11Texture2D** ppShaderResource);


//
//   PURPOSE: Create a shader-resource view for accessing data in a resource.
//
void CreateShaderResourceView(ID3D11Device* pDevice, ID3D11Texture2D* pShaderResource, ID3D11ShaderResourceView** ppShaderResourceView);


//
//   PURPOSE: Creates a buffer for vertex data.
//
template<typename T>
inline void CreateVertexBuffer(ID3D11Device* pDevice, std::vector<T>* pVertices, UINT ByteWidth, ID3D11Buffer** ppBuffer)
{
	D3D11_BUFFER_DESC Desc;
	Desc.ByteWidth = ByteWidth;
	Desc.Usage = D3D11_USAGE_DEFAULT;
	Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	Desc.CPUAccessFlags = 0;															// No CPU access is necessary.
	Desc.MiscFlags = 0;
	Desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitialData;
	InitialData.pSysMem = &(*pVertices)[0];

	if (FAILED(pDevice->CreateBuffer(&Desc, &InitialData, ppBuffer)))
		throw std::exception("CreateBuffer Failed");
}


//
//   PURPOSE: Retrieves the current directory for the executable file.
//
std::string GetCurrentPathA();
std::wstring GetCurrentPathW();