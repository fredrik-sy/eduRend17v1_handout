#include "stdafx.h"
#include "Application.h"
#include "Common/Direct3D.h"
#include "Models/Sphere.h"
#include "Models/Sponza.h"
#include "Common/Timer.h"
#include "Buffers/PhongBuffer.h"
#include "Models/WoodDoll.h"


Application::Application(HINSTANCE hInstance, WNDPROC lpfnWndProc)
	: Window("eduRend", "eduRend", hInstance, lpfnWndProc)
{
	CreateDeviceAndSwapChain(GetWindowHandle(), &m_pDevice, &m_pDeviceContext, &m_pSwapChain);
	CreateRasterizerState(m_pDevice, &m_pRasterizerState);
	CreateSamplerState(m_pDevice, &m_pSamplerState);

	ID3DBlob* pCode;
	D3D11_INPUT_ELEMENT_DESC InputElementDescs[] = {
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEX",		0, DXGI_FORMAT_R32G32_FLOAT,	0, 48,	D3D11_INPUT_PER_VERTEX_DATA, 0 } };

#pragma region [0] Common Shader
	CreateDepthStencilResource(m_pDevice, GetClientWidth(), GetClientHeight(), &m_pDepthStencilResources[0]);
	CreateDepthStencilView(m_pDevice, m_pDepthStencilResources[0], &m_pDepthStencilViews[0]);
	CreateRenderTargetView(m_pDevice, m_pSwapChain, &m_pRenderTargetViews[0]);

	CompileShader(L"../assets/shaders/DrawTri.vs.hlsl", "VS_main", "vs_5_0", &pCode);
	CreateVertexShader(m_pDevice, pCode, &m_pVertexShaders[0]);
	CreateInputLayout(m_pDevice, InputElementDescs, ARRAYSIZE(InputElementDescs), pCode, &m_pInputLayouts[0]);
	SAFE_RELEASE(pCode);

	CompileShader(L"../assets/shaders/DrawTri.ps.hlsl", "PS_main", "ps_5_0", &pCode);
	CreatePixelShader(m_pDevice, pCode, &m_pPixelShaders[0]);
	SAFE_RELEASE(pCode);
#pragma endregion

#pragma region [1-6] Shadow Mapping Shader
	CreateShaderResource(m_pDevice, GetClientWidth(), GetClientHeight(), &m_pShaderResources[0]);
	CreateShaderResourceView(m_pDevice, m_pShaderResources[0], &m_pShaderResourceViews[0]);
	CreateDepthStencilResource(m_pDevice, GetClientWidth(), GetClientHeight(), &m_pDepthStencilResources[1]);
	CreateDepthStencilView(m_pDevice, m_pDepthStencilResources[1], &m_pDepthStencilViews[1]);
	CreateRenderTargetView(m_pDevice, m_pSwapChain, m_pShaderResources[0], &m_pRenderTargetViews[1]);

	CompileShader(L"../assets/shaders/ShadowMapping.vs.hlsl", "VS_main", "vs_5_0", &pCode);
	CreateVertexShader(m_pDevice, pCode, &m_pVertexShaders[1]);
	CreateInputLayout(m_pDevice, InputElementDescs, ARRAYSIZE(InputElementDescs), pCode, &m_pInputLayouts[1]);
	SAFE_RELEASE(pCode);

	CompileShader(L"../assets/shaders/ShadowMapping.ps.hlsl", "PS_main", "ps_5_0", &pCode);
	CreatePixelShader(m_pDevice, pCode, &m_pPixelShaders[1]);
	SAFE_RELEASE(pCode);
#pragma endregion

	// Continue initialize until it succeeds, since this will fail if window has no focus.
	while (!m_InputHandler.Initialize(hInstance, GetWindowHandle(), GetClientWidth(), GetClientHeight()))
		Sleep(1000);
}


Application::~Application()
{
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pRasterizerState);
	SAFE_RELEASE(m_pSamplerState);
	SAFE_RELEASE(m_pMatrixBuffer);
	SAFE_RELEASE(m_pPositionBuffer);
	SAFE_RELEASE(m_pPhongBuffer);

	for (ID3D11DepthStencilView* pDepthStencilView : m_pDepthStencilViews)
		SAFE_RELEASE(pDepthStencilView);

	for (ID3D11Texture2D* pDepthStencilResource : m_pDepthStencilResources)
		SAFE_RELEASE(pDepthStencilResource);

	for (ID3D11RenderTargetView* pRenderTargetView : m_pRenderTargetViews)
		SAFE_RELEASE(pRenderTargetView);

	for (ID3D11VertexShader* pVertexShader : m_pVertexShaders)
		SAFE_RELEASE(pVertexShader);

	for (ID3D11PixelShader* pPixelShader : m_pPixelShaders)
		SAFE_RELEASE(pPixelShader);

	for (ID3D11InputLayout* pInputLayout : m_pInputLayouts)
		SAFE_RELEASE(pInputLayout);

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
	CreateConstantBuffer(m_pDevice, sizeof(MatrixBuffer), &m_pMatrixBuffer);
	CreateConstantBuffer(m_pDevice, sizeof(PositionBuffer), &m_pPositionBuffer);
	CreateConstantBuffer(m_pDevice, sizeof(PhongBuffer), &m_pPhongBuffer);

	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		// How the pipeline interprets vertex data that is bound to the input-assembler stage. Different topology can be used for different vertex data.
	m_pDeviceContext->RSSetState(m_pRasterizerState);										// Set the rasterizer state for the rasterizer stage of the pipeline.
	m_pDeviceContext->RSSetViewports(1, &CreateSingleViewport());							// Bind viewport to the rasterizer stage of the pipeline.
	m_pDeviceContext->PSSetSamplers(0, 1, &m_pSamplerState);								// Set samplers to the pixel shader pipeline stage.

	// Set buffers used by the pipeline stage.
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pMatrixBuffer);
	m_pDeviceContext->PSSetConstantBuffers(1, 1, &m_pPositionBuffer);
	m_pDeviceContext->PSSetConstantBuffers(2, 1, &m_pPhongBuffer);
}


void Application::LoadContent()
{
	m_GameObjects.push_back(new Sponza(m_pDevice, m_pDeviceContext, m_pPhongBuffer));
	m_GameObjects.push_back(new Sphere(m_pDevice, m_pDeviceContext, m_pPhongBuffer));
	m_GameObjects.push_back(new WoodDoll(m_pDevice, m_pDeviceContext, m_pPhongBuffer));
	m_Camera.SetAspectRatio(GetAspectRatio());
	m_Camera.SetPosition(0.0f, 0.0f, 5.0f);
	m_PointLight.SetAspectRatio(GetAspectRatio());
	m_PointLight.SetPosition(0.0f, 5.0f, -20.0f);

	m_PositionData.LightPosition = vec3f(0.0f, 5.0f, -20.0f);
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
		pGameObject->Update(DeltaTime);

	m_PositionData.CameraPosition = m_Camera.GetPosition();
}


void Application::Render(float DeltaTime)
{
	RenderShadowMapping(DeltaTime);

	m_pDeviceContext->IASetInputLayout(m_pInputLayouts[0]);											// Bind to input-assembler stage.
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetViews[0], m_pDepthStencilViews[0]);		// Bind render targets and the depth-stencil buffer to the output-merger stage.

	// Clear render view and depth-stencil resource.
	static const FLOAT RGBA[4] = { 0, 0, 0, 1 };				// Black color.
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetViews[0], RGBA);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilViews[0], D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	// Set shaders to the device.
	m_pDeviceContext->VSSetShader(m_pVertexShaders[0], NULL, 0);
	m_pDeviceContext->HSSetShader(NULL, NULL, 0);				// Hull shader. Passing NULL disables the shader for this pipeline stage.
	m_pDeviceContext->DSSetShader(NULL, NULL, 0);				// Domain shader.
	m_pDeviceContext->GSSetShader(NULL, NULL, 0);				// Geometry shader.
	m_pDeviceContext->PSSetShader(m_pPixelShaders[0], NULL, 0);
	m_pDeviceContext->PSSetShaderResources(5, 1, &m_pShaderResourceViews[0]);

	m_MatrixData.Projection = m_Camera.GetProjectionMatrix();
	m_MatrixData.WorldToView = m_Camera.GetWorldToViewMatrix();

	MapUpdateAndUnmapSubresource(m_pDeviceContext, m_pPositionBuffer, &m_PositionData, sizeof(PositionBuffer));

	for (GameObject* pGameObject : m_GameObjects)
	{
		m_MatrixData.ModelToWorld = pGameObject->GetTransformationMatrix();
		MapUpdateAndUnmapSubresource(m_pDeviceContext, m_pMatrixBuffer, &m_MatrixData, sizeof(MatrixBuffer));
		pGameObject->Render(m_pDeviceContext);
	}

	m_pSwapChain->Present(0, 0);									// Presents a rendered image to the user.
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


void Application::RenderShadowMapping(float DeltaTime)
{
	m_pDeviceContext->IASetInputLayout(m_pInputLayouts[1]);											// Bind to input-assembler stage.
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetViews[1], m_pDepthStencilViews[1]);		// Bind render targets and the depth-stencil buffer to the output-merger stage.

	// Clear render view and depth-stencil resource.
	static const FLOAT RGBA[4] = { 0, 0, 0, 1 };				// Black color.
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetViews[1], RGBA);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilViews[1], D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	// Set shaders to the device.
	m_pDeviceContext->VSSetShader(m_pVertexShaders[1], NULL, 0);
	m_pDeviceContext->HSSetShader(NULL, NULL, 0);				// Hull shader. Passing NULL disables the shader for this pipeline stage.
	m_pDeviceContext->DSSetShader(NULL, NULL, 0);				// Domain shader.
	m_pDeviceContext->GSSetShader(NULL, NULL, 0);				// Geometry shader.
	m_pDeviceContext->PSSetShader(m_pPixelShaders[1], NULL, 0);

	MapUpdateAndUnmapSubresource(m_pDeviceContext, m_pPositionBuffer, &m_PositionData, sizeof(PositionBuffer));

	m_MatrixData.Projection = m_PointLight.GetProjectionMatrix();
	m_MatrixData.WorldToView = m_PointLight.GetWorldToViewMatrix(POINT_LIGHT_FACE_FORWARD);

	for (GameObject* pGameObject : m_GameObjects)
	{
		m_MatrixData.ModelToWorld = pGameObject->GetTransformationMatrix();
		MapUpdateAndUnmapSubresource(m_pDeviceContext, m_pMatrixBuffer, &m_MatrixData, sizeof(MatrixBuffer));
		pGameObject->Render(m_pDeviceContext);
	}

	m_pSwapChain->Present(0, 0);									// Presents a rendered image to the user.
}


void Application::OnResize()
{
	m_pDeviceContext->OMSetRenderTargets(0, NULL, NULL);		// Unbind render targets and the depth-stencil from the pipeline.

	if (FAILED(m_pSwapChain->ResizeBuffers(
		0,				// Preserve the existing number of buffers in the swap chain.
		0,				// Use the width of the client area of the window, zero value can't be used if you called the IDXGIFactory2::CreateSwapChainForComposition method to create the swap chain.
		0,				// Use the height of the client area of the window.
		DXGI_FORMAT_UNKNOWN,
		0)))
		throw std::exception("ResizeBuffers Failed");

	for (unsigned int i = 0; i < MAX_RENDER_TARGETS_LEN; ++i)
	{
		SAFE_RELEASE(m_pRenderTargetViews[i]);
		SAFE_RELEASE(m_pDepthStencilResources[i]);
		SAFE_RELEASE(m_pDepthStencilViews[i]);

		CreateRenderTargetView(m_pDevice, m_pSwapChain, &m_pRenderTargetViews[i]);
		CreateDepthStencilResource(m_pDevice, GetClientWidth(), GetClientHeight(), &m_pDepthStencilResources[i]);
		CreateDepthStencilView(m_pDevice, m_pDepthStencilResources[i], &m_pDepthStencilViews[i]);
	}

	m_pDeviceContext->RSSetViewports(1, &CreateSingleViewport());						// Bind viewport to the rasterizer stage of the pipeline.
	m_Camera.SetAspectRatio(GetAspectRatio());
	m_PointLight.SetAspectRatio(GetAspectRatio());
}
