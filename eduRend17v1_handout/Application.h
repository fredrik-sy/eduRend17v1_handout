#pragma once

#include <d3d11.h>
#include <vector>
#include "Common/Camera.h"
#include "Common/Window.h"
#include "Models/GameObject.h"
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
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	ID3D11InputLayout* m_pInputLayout;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11RasterizerState* m_pRasterizerState;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11Texture2D* m_pDepthStencilResource;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11SamplerState* m_pSamplerState;
	IDXGISwapChain* m_pSwapChain;
	
	Camera m_Camera;
	InputHandler m_InputHandler;
	vector<GameObject*> m_GameObjects;

	D3D11_VIEWPORT CreateSingleViewport();
	void OnResize();


	////////////////////////////////////////////////////////////////////////////////////////////////////


public:
private:
	ID3D11Buffer* m_pMatrixBuffer;
	ID3D11Buffer* m_pPositionBuffer;
	MatrixBuffer m_MatrixData;
	PositionBuffer m_PositionData;

};

