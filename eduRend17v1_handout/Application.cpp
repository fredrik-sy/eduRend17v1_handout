#include "stdafx.h"
#include "Application.h"
#include "Common/Direct3D.h"
#include "Models/Sphere.h"
#include "Models/Sponza.h"
#include "Common/Timer.h"
#include "Buffers/MaterialBuffer.h"

Application::Application(HINSTANCE hInstance, WNDPROC lpfnWndProc)
	: Window("eduRend", "eduRend", hInstance, lpfnWndProc)
{
	CreateDeviceAndSwapChain(GetWindowHandle(), &m_pDevice, &m_pDeviceContext, &m_pSwapChain);
	CreateRenderTargetView(m_pDevice, m_pSwapChain, &m_pRenderTargetView);
	CreateDepthStencilResource(m_pDevice, GetClientWidth(), GetClientHeight(), &m_pDepthStencilResource);
	CreateDepthStencilView(m_pDevice, m_pDepthStencilResource, &m_pDepthStencilView);
	CreateRasterizerState(m_pDevice, &m_pRasterizerState);

	ID3DBlob* pCode;
	D3D11_INPUT_ELEMENT_DESC InputElementDescs[] = {
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEX",		0, DXGI_FORMAT_R32G32_FLOAT,	0, 48,	D3D11_INPUT_PER_VERTEX_DATA, 0 } };

	CompileShader(L"../assets/shaders/DrawTri.vs.hlsl", "VS_main", "vs_5_0", &pCode);
	CreateVertexShader(m_pDevice, pCode, &m_pVertexShader);
	CreateInputLayout(m_pDevice, InputElementDescs, ARRAYSIZE(InputElementDescs), pCode, &m_pInputLayout);
	SAFE_RELEASE(pCode);

	CompileShader(L"../assets/shaders/DrawTri.ps.hlsl", "PS_main", "ps_5_0", &pCode);
	CreatePixelShader(m_pDevice, pCode, &m_pPixelShader);
	SAFE_RELEASE(pCode);

	m_InputHandler.Initialize(hInstance, GetWindowHandle(), GetClientWidth(), GetClientHeight());
}


Application::~Application()
{
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDepthStencilResource);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pInputLayout);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pRasterizerState);
	//SAFE_RELEASE(m_pMatrixBuffer);
	SAFE_RELEASE(m_pMatrixBufferA);

	for (GameObject* pGameObject : m_GameObjects)
		SAFE_DELETE(pGameObject);
}


void Application::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:
		if (m_pSwapChain)
		{
			switch (wParam)
			{
			case SIZE_MAXIMIZED:
			case SIZE_RESTORED:
				OnResize();
				break;
			default:
				break;
			}
		}
		break;
	}
}

void Application::Run()
{
	MSG msg = { 0 };

	Initialize();
	LoadContent();
	Timer PreviousTime = Timer::Now();

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(
			&msg,
			NULL,				// Process both window message and thread message.
			0,					// No range filtering.
			0,
			PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Timer CurrentTime = Timer::Now();
			float DeltaTime = (CurrentTime - PreviousTime).Seconds();
			Update(DeltaTime);
			Render(DeltaTime);
			PreviousTime = CurrentTime;
		}
	}

	UnloadContent();
}

void Application::Initialize()
{
	CreateConstantBuffer(m_pDevice, sizeof(MatrixBufferA), &m_pMatrixBufferA);
	CreateConstantBuffer(m_pDevice, sizeof(PositionBufferA), &m_pPositionBufferA);
	CreateConstantBuffer(m_pDevice, sizeof(MaterialBufferA), &m_pMaterialBufferA);

	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		// How the pipeline interprets vertex data that is bound to the input-assembler stage. Different topology can be used for different vertex data.
	m_pDeviceContext->IASetInputLayout(m_pInputLayout);										// Bind to input-assembler stage.
	m_pDeviceContext->RSSetState(m_pRasterizerState);										// Set the rasterizer state for the rasterizer stage of the pipeline.
	m_pDeviceContext->RSSetViewports(1, &CreateSingleViewport());							// Bind viewport to the rasterizer stage of the pipeline.
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);		// Bind render targets and the depth-stencil buffer to the output-merger stage.
}


void Application::LoadContent()
{
	m_GameObjects.push_back(new Sponza(m_pDevice, m_pDeviceContext));
	m_GameObjects.push_back(new Sphere(m_pDevice, m_pDeviceContext));
	m_Camera.SetAspectRatio(GetAspectRatio());
	m_Camera.SetPosition(0.0f, 0.0f, 5.0f);
	m_PositionDataA.LightPosition = vec3f(0.0f, 50.0f, -30.0f);
}


void Application::Update(float DeltaTime)
{
	m_InputHandler.Update();
	m_Camera.Rotate(-m_InputHandler.GetMouseDeltaX() * DeltaTime, -m_InputHandler.GetMouseDeltaY() * DeltaTime, 0);

	if (m_InputHandler.IsKeyPressed(Keys::Up)) m_Camera.Rotate(0.0f, DeltaTime, 0.0f);
	if (m_InputHandler.IsKeyPressed(Keys::Down)) m_Camera.Rotate(0.0f, -DeltaTime, 0.0f);
	if (m_InputHandler.IsKeyPressed(Keys::Left)) m_Camera.Rotate(DeltaTime, 0.0f, 0.0f);
	if (m_InputHandler.IsKeyPressed(Keys::Right)) m_Camera.Rotate(-DeltaTime, 0.0f, 0.0f);

	if (m_InputHandler.IsKeyPressed(Keys::W)) m_Camera.Move(0.0f, 0.0f, -10.0f * DeltaTime);
	if (m_InputHandler.IsKeyPressed(Keys::S)) m_Camera.Move(0.0f, 0.0f, 10.0f * DeltaTime);
	if (m_InputHandler.IsKeyPressed(Keys::D)) m_Camera.Move(10.0f * DeltaTime, 0.0f, 0.0f);
	if (m_InputHandler.IsKeyPressed(Keys::A)) m_Camera.Move(-10.0f * DeltaTime, 0.0f, 0.0f);

	for (GameObject* pGameObject : m_GameObjects)
		pGameObject->Update();

	//m_MatrixData.Projection = m_Camera.GetProjectionMatrix();
	//m_MatrixData.WorldToView = m_Camera.GetWorldToViewMatrix();
	m_MatrixDataA.Projection = m_Camera.GetProjectionMatrixA();
	m_MatrixDataA.WorldToView = m_Camera.GetWorldToViewMatrixA();
	m_PositionDataA.CameraPosition = m_Camera.GetPosition();
}


void Application::Render(float DeltaTime)
{
	// Clear render view and depth-stencil resource.
	static const FLOAT rgba[4] = { 0, 0, 0, 1 };				// Black color.
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, rgba);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	// Set shaders to the device.
	m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pDeviceContext->HSSetShader(NULL, NULL, 0);				// Hull shader. Passing NULL disables the shader for this pipeline stage.
	m_pDeviceContext->DSSetShader(NULL, NULL, 0);				// Domain shader.
	m_pDeviceContext->GSSetShader(NULL, NULL, 0);				// Geometry shader.
	m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	// Set buffers used by the shader pipeline stage.
	//m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pMatrixBuffer);
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pMatrixBufferA);
	m_pDeviceContext->PSSetConstantBuffers(1, 1, &m_pPositionBufferA);
	m_pDeviceContext->PSSetConstantBuffers(2, 1, &m_pMaterialBufferA);

	MapUpdateAndUnmapSubresource(m_pDeviceContext, m_pPositionBufferA, &m_PositionDataA, sizeof(PositionBufferA));

	for (GameObject* pGameObject : m_GameObjects)
	{
		//m_MatrixData.ModelToWorld = pGameObject->GetTransformationMatrix();
		//MapUpdateAndUnmapSubresource(m_pDeviceContext, m_pMatrixBuffer, &m_MatrixData, sizeof(MatrixBuffer));
		m_MatrixDataA.ModelToWorld = pGameObject->GetTransformationMatrixA();
		MapUpdateAndUnmapSubresource(m_pDeviceContext, m_pMatrixBufferA, &m_MatrixDataA, sizeof(MatrixBufferA));
		MapUpdateAndUnmapSubresource(m_pDeviceContext, m_pMaterialBufferA, &pGameObject->GetMaterialDataA(), sizeof(MaterialBufferA));
		pGameObject->Render(m_pDeviceContext);
	}

	m_pSwapChain->Present(0, 0);								// Presents a rendered image to the user.
}


void Application::UnloadContent()
{
}


D3D11_VIEWPORT Application::CreateSingleViewport()
{
	D3D11_VIEWPORT Viewport;
	Viewport.Width = (FLOAT)GetClientWidth();
	Viewport.Height = (FLOAT)GetClientHeight();
	Viewport.MinDepth = 0;
	Viewport.MaxDepth = 1;
	Viewport.TopLeftX = 0;
	Viewport.TopLeftY = 0;
	return Viewport;
}


void Application::OnResize()
{
	m_pDeviceContext->OMSetRenderTargets(0, NULL, NULL);		// Unbind render targets and the depth-stencil from the pipeline.

	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDepthStencilResource);
	SAFE_RELEASE(m_pDepthStencilView);

	if (FAILED(m_pSwapChain->ResizeBuffers(
		0,				// Preserve the existing number of buffers in the swap chain.
		0,				// Use the width of the client area of the window, zero value can't be used if you called the IDXGIFactory2::CreateSwapChainForComposition method to create the swap chain.
		0,				// Use the height of the client area of the window.
		DXGI_FORMAT_UNKNOWN,
		0)))
		throw std::exception("ResizeBuffers Failed");

	CreateRenderTargetView(m_pDevice, m_pSwapChain, &m_pRenderTargetView);
	CreateDepthStencilResource(m_pDevice, GetClientWidth(), GetClientHeight(), &m_pDepthStencilResource);
	CreateDepthStencilView(m_pDevice, m_pDepthStencilResource, &m_pDepthStencilView);

	m_pDeviceContext->RSSetViewports(1, &CreateSingleViewport());								// Bind viewport to the rasterizer stage of the pipeline.
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);			// Bind render targets and the depth-stencil to the pipeline.
	m_Camera.SetAspectRatio(GetAspectRatio());
}
