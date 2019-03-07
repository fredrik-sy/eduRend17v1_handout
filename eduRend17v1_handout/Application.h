#pragma once

#include <d3d11.h>
#include <vector>
#include "Common/Camera.h"
#include "Common/Window.h"
#include "Models/GameObject.h"
#include "Models/PointLight.h"
#include "Buffers/MatrixBuffer.h"
#include "Buffers/PositionBuffer.h"
#include "source/InputHandler.h"

using std::vector;

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
	ID3D11InputLayout* m_pInputLayouts[2];			// 0 - Common Shader, 1 - Shadow Mapping Shader.
	ID3D11PixelShader* m_pPixelShaders[2];
	ID3D11RasterizerState* m_pRasterizerState;
	ID3D11RenderTargetView* m_pRenderTargetViews[2];
	ID3D11Texture2D* m_pDepthStencilResources[2];
	ID3D11DepthStencilView* m_pDepthStencilViews[2];
	ID3D11VertexShader* m_pVertexShaders[2];
	ID3D11SamplerState* m_pSamplerState;
	IDXGISwapChain* m_pSwapChain;

	//
	// Shadow Mapping
	//
	ID3D11Texture2D* m_pShaderResources[1];
	ID3D11ShaderResourceView* m_pShaderResourceViews[1];

	//
	// Buffers
	//
	ID3D11Buffer* m_pMatrixBuffer;
	ID3D11Buffer* m_pPositionBuffer;
	ID3D11Buffer* m_pPhongBuffer;
	MatrixBuffer m_MatrixData;
	PositionBuffer m_PositionData;

	Camera m_Camera;
	PointLight m_PointLight;
	InputHandler m_InputHandler;
	vector<GameObject*> m_GameObjects;

	D3D11_VIEWPORT CreateSingleViewport();
	void RenderShadowMapping(float DeltaTime);
	void OnResize();

};

