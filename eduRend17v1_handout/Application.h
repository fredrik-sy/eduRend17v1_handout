#pragma once

#include <d3d11.h>
#include <vector>
#include "Common/Camera.h"
#include "Common/Window.h"
#include "Models/GameObject.h"
#include "Models/DirectionalLight.h"
#include "Buffers/MatrixBuffer.h"
#include "Buffers/PositionBuffer.h"
#include "Buffers/LightMatrixBuffer.h"
#include "source/InputHandler.h"

using std::vector;

#define SHADER_RESOURCE_LEN 1
#define DEPTH_STENCIL_LEN (1 + SHADER_RESOURCE_LEN)
#define INPUT_LEN 2
#define SHADOW_MAPPING_WIDTH 1024
#define SHADOW_MAPPING_HEIGHT 1024

class Application : private Window
{
public:
	Application(HINSTANCE hInstance, WNDPROC lpfnWndProc);
	virtual ~Application();

	void MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void Run();

	void Initialize();
	void LoadContent();
	void Update(float DeltaTime);
	void Render(float DeltaTime);
	void UnloadContent();

private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	ID3D11InputLayout* m_pInputLayouts[INPUT_LEN];
	ID3D11PixelShader* m_pPixelShaders[INPUT_LEN];
	ID3D11RasterizerState* m_pRasterizerState;
	ID3D11RenderTargetView* m_pRenderTargetViews[DEPTH_STENCIL_LEN];
	ID3D11Texture2D* m_pDepthStencilBuffers[DEPTH_STENCIL_LEN];
	ID3D11DepthStencilView* m_pDepthStencilViews[DEPTH_STENCIL_LEN];
	ID3D11VertexShader* m_pVertexShaders[INPUT_LEN];
	ID3D11SamplerState* m_pSamplerState;
	ID3D11SamplerState* m_pComparisonSamplerState;
	IDXGISwapChain* m_pSwapChain;

	//
	// Shadow Mapping
	//
	ID3D11Texture2D* m_pShaderResourceBuffers[SHADER_RESOURCE_LEN];
	ID3D11ShaderResourceView* m_pShaderResourceViews[SHADER_RESOURCE_LEN];

	//
	// Buffers
	//
	ID3D11Buffer* m_pMatrixBuffer;
	ID3D11Buffer* m_pPositionBuffer;
	ID3D11Buffer* m_pPhongBuffer;
	ID3D11Buffer* m_pLightMatrixBuffer;
	MatrixBuffer m_MatrixData;
	PositionBuffer m_PositionData;
	LightMatrixBuffer m_LightMatrixData;

	Camera m_Camera;
	DirectionalLight m_DirectionalLight;
	InputHandler m_InputHandler;
	vector<GameObject*> m_GameObjects;

	D3D11_VIEWPORT CreateSingleViewport();
	void OnResize();

};

