#include "stdafx.h"
#include "Direct3D.h"
#include <d3dcompiler.h>
#include <exception>


void CreateDeviceAndSwapChain(HWND OutputWindow, ID3D11Device** ppDevice, ID3D11DeviceContext** ppDeviceContext, IDXGISwapChain** ppSwapChain)
{
	UINT Flags = 0;
	D3D_FEATURE_LEVEL FeatureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
	D3D_DRIVER_TYPE DriverType = D3D_DRIVER_TYPE_HARDWARE;

#ifdef _DEBUG
	//Flags |= D3D11_CREATE_DEVICE_DEBUG;
	//Flags |= D3D11_CREATE_DEVICE_DEBUGGABLE;
#endif

	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	SwapChainDesc.BufferDesc.Width = 0;
	SwapChainDesc.BufferDesc.Height = 0;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChainDesc.SampleDesc.Count = 1;													// No anti-aliasing.
	SwapChainDesc.SampleDesc.Quality = 0;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.OutputWindow = OutputWindow;
	SwapChainDesc.Windowed = TRUE;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	SwapChainDesc.Flags = 0;

	if (FAILED(D3D11CreateDeviceAndSwapChain(
		NULL,																			// Use the default video adapter.
		DriverType,
		NULL,																			// Skip a handle to a DLL that implements a software rasterizer.
		Flags,
		FeatureLevels,
		ARRAYSIZE(FeatureLevels),
		D3D11_SDK_VERSION,
		&SwapChainDesc,
		ppSwapChain,
		ppDevice,
		NULL,																			// Don't need to determine which feature level is supported since we only use one.
		ppDeviceContext)))
		throw std::exception("D3D11CreateDeviceAndSwapChain Failed");
}


void CreateRenderTargetView(ID3D11Device* pDevice, IDXGISwapChain* pSwapChain, ID3D11RenderTargetView** ppRenderTargetView)
{
	ID3D11Texture2D* pBuffer;

	if (FAILED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer)))
		throw std::exception("GetBuffer Failed");

	HRESULT hr = pDevice->CreateRenderTargetView(
		pBuffer,
		NULL,																			// Create a view that can access all of the subresources in mipmap level 0.
		ppRenderTargetView);

	SAFE_RELEASE(pBuffer);

	if (FAILED(hr))
		throw std::exception("CreateRenderTargetView Failed");
}


void CreateDepthStencilResource(ID3D11Device* pDevice, UINT Width, UINT Height, ID3D11Texture2D** ppDepthStencilResource)
{
	D3D11_TEXTURE2D_DESC Desc;
	Desc.Width = Width;
	Desc.Height = Height;
	Desc.MipLevels = 1;																	// 1 for multisampled texture or 0 to generate a full set of subtextures.
	Desc.ArraySize = 1;																	// Number of textures in the array.
	Desc.Format = DXGI_FORMAT_D32_FLOAT;
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;
	Desc.Usage = D3D11_USAGE_DEFAULT;
	Desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	Desc.CPUAccessFlags = 0;															// CPU access is not required.
	Desc.MiscFlags = 0;

	if (FAILED(pDevice->CreateTexture2D(
		&Desc,
		NULL,
		ppDepthStencilResource)))
		throw std::exception("CreateTexture2D Failed");
}


void CreateDepthStencilView(ID3D11Device* pDevice, ID3D11Texture2D* pDepthStencilResource, ID3D11DepthStencilView** ppDepthStencilView)
{
	D3D11_DEPTH_STENCIL_VIEW_DESC Desc = { };
	Desc.Format = DXGI_FORMAT_D32_FLOAT;
	Desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	Desc.Texture2D.MipSlice = 0;

	if (FAILED(pDevice->CreateDepthStencilView(
		pDepthStencilResource,
		&Desc,
		ppDepthStencilView)))
		throw std::exception("CreateDepthStencilView Failed");
}


void CreateRasterizerState(ID3D11Device* pDevice, ID3D11RasterizerState** ppRasterizerState)
{
	D3D11_RASTERIZER_DESC RasterizerDesc;
	RasterizerDesc.FillMode = D3D11_FILL_SOLID;
	RasterizerDesc.CullMode = D3D11_CULL_BACK;											// Don't draw triangles that are back-facing.
	RasterizerDesc.FrontCounterClockwise = TRUE;										// A triangle will be front-facing if its vertices are counter-clockwise.
	RasterizerDesc.DepthBias = FALSE;
	RasterizerDesc.DepthBiasClamp = 0;
	RasterizerDesc.SlopeScaledDepthBias = 0;
	RasterizerDesc.DepthClipEnable = TRUE;												// Enable clipping based on distance.
	RasterizerDesc.ScissorEnable = FALSE;												// Pixels outside an active scissor rectangle are culled.
	RasterizerDesc.MultisampleEnable = FALSE;											// FALSE to use alpha line anti-aliasing algorithm.
	RasterizerDesc.AntialiasedLineEnable = FALSE;

	if (FAILED(pDevice->CreateRasterizerState(&RasterizerDesc, ppRasterizerState)))
		throw std::exception("CreateRasterizerState Failed");
}


void CreateSamplerState(ID3D11Device * pDevice, ID3D11SamplerState ** ppSamplerState)
{
	D3D11_SAMPLER_DESC SamplerDesc;
	SamplerDesc.Filter = D3D11_FILTER_COMPARISON_ANISOTROPIC;							// Filtering method to use when sampling a texture.
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;									// Method to use for resolving a texture coordinate that is outside the 0 to 1 range.
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.MipLODBias = 0.0f;														// Offset from the calculated mipmap level.
	SamplerDesc.MaxAnisotropy = 1;														// Clamping value used if D3D11_FILTER_ANISOTROPIC or D3D11_FILTER_COMPARISON_ANISOTROPIC is specified in Filter.
	SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;								// Compares sampled data against existing sampled data.
	SamplerDesc.BorderColor[0] = 1.0f;
	SamplerDesc.BorderColor[1] = 1.0f;
	SamplerDesc.BorderColor[2] = 1.0f;
	SamplerDesc.BorderColor[3] = 1.0f;
	SamplerDesc.MinLOD = -FLT_MAX;
	SamplerDesc.MaxLOD = FLT_MAX;

	if (FAILED(pDevice->CreateSamplerState(&SamplerDesc, ppSamplerState)))
		throw std::exception("CreateSamplerState Failed");
}


void CreateVertexShader(ID3D11Device* pDevice, ID3DBlob* pCode, ID3D11VertexShader** ppVertexShader)
{
	if (FAILED(pDevice->CreateVertexShader(
		pCode->GetBufferPointer(),
		pCode->GetBufferSize(),
		NULL,																			// HLSL dynamic linkage not used.
		ppVertexShader)))
		throw std::exception("CreateVertexShader Failed");
}


void CreateInputLayout(ID3D11Device* pDevice, const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, UINT NumElements, ID3DBlob* pCode, ID3D11InputLayout** ppInputLayout)
{
	if (pDevice->CreateInputLayout(
		pInputElementDescs,
		NumElements,
		pCode->GetBufferPointer(),
		pCode->GetBufferSize(),
		ppInputLayout))
		throw std::exception("CreateInputLayout Failed");
}


void CreatePixelShader(ID3D11Device* pDevice, ID3DBlob* pCode, ID3D11PixelShader** ppPixelShader)
{
	if (FAILED(pDevice->CreatePixelShader(
		pCode->GetBufferPointer(),
		pCode->GetBufferSize(),
		NULL,
		ppPixelShader)))
		throw std::exception("CreatePixelShader Failed");
}


void CompileShader(LPCWSTR pFileName, LPCSTR pEntrypoint, LPCSTR pTarget, ID3DBlob** ppCode)
{
	UINT Flags1 = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_IEEE_STRICTNESS;
	ID3DBlob* pErrorMsgs;

#ifdef _DEBUG
	Flags1 |= D3DCOMPILE_DEBUG;
#endif

	if (FAILED(D3DCompileFromFile(
		pFileName,
		NULL,																			// Shader macros not used.
		NULL,																			// Include files not used.
		pEntrypoint,
		pTarget,
		Flags1,
		0,																				// D3DCompileFromFile ignores Flags2.
		ppCode,
		&pErrorMsgs)))
		throw std::exception("D3DCompileFromFile Failed");

	SAFE_RELEASE(pErrorMsgs);
}


void CreateConstantBuffer(ID3D11Device* pDevice, UINT ByteWidth, ID3D11Buffer** ppBuffer)
{
	D3D11_BUFFER_DESC Desc;
	Desc.ByteWidth = ByteWidth;															// ByteWidth value needs to be in multiples of 16.
	Desc.Usage = D3D11_USAGE_DYNAMIC;													// Make resource accessible by both the GPU (read only) and the CPU (write only).
	Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Desc.MiscFlags = 0;
	Desc.StructureByteStride = 0;

	if (FAILED(pDevice->CreateBuffer(&Desc, NULL, ppBuffer)))
		throw std::exception("CreateBuffer Failed");
}


void CreateIndexBuffer(ID3D11Device* pDevice, std::vector<unsigned int>* pIndices, ID3D11Buffer** ppBuffer)
{
	D3D11_BUFFER_DESC Desc;
	Desc.ByteWidth = sizeof(unsigned int) * pIndices->size();
	Desc.Usage = D3D11_USAGE_DEFAULT;
	Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	Desc.CPUAccessFlags = 0;															// No CPU access is necessary.
	Desc.MiscFlags = 0;
	Desc.StructureByteStride = 0;
	
	D3D11_SUBRESOURCE_DATA InitialData;
	InitialData.pSysMem = &(*pIndices)[0];

	if (FAILED(pDevice->CreateBuffer(&Desc, &InitialData, ppBuffer)))
		throw std::exception("CreateBuffer Failed");
}


void MapUpdateAndUnmapSubresource(ID3D11DeviceContext* pDeviceContext, ID3D11Resource* pResource, void* pData, UINT Size)
{
	D3D11_MAPPED_SUBRESOURCE Subresource;

	if (FAILED(pDeviceContext->Map(pResource, 0, D3D11_MAP_WRITE_DISCARD, 0, &Subresource)))
		throw std::exception("Map Failed");

	memcpy(Subresource.pData, pData, Size);
	pDeviceContext->Unmap(pResource, 0);
}


void CreateShaderResourceView(ID3D11Device* pDevice, ID3D11Resource* pResource, ID3D11ShaderResourceView** ppShaderResourceView)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC Desc;
	Desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	Desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	Desc.Texture2D.MipLevels = 0;

	if (FAILED(pDevice->CreateShaderResourceView(pResource, &Desc, ppShaderResourceView)))
		throw std::exception("CreateBuffer Failed");
}
