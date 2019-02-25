#include "stdafx.h"
#include "Window.h"
#include <exception>


Window::Window(const char Title[], const char ClassName[], HINSTANCE hInstance, WNDPROC lpfnWndProc)
{
	unsigned int TitleSize = strlen(Title) + 1;
	unsigned int ClassNameSize = strlen(ClassName) + 1;

	m_pTitle = new char[TitleSize];
	m_pClassName = new char[ClassNameSize];

	strcpy_s(m_pTitle, TitleSize, Title);
	strcpy_s(m_pClassName, ClassNameSize, ClassName);

	RegisterWindowClass(hInstance, lpfnWndProc);
	CreateWindowFrame(hInstance);
}


Window::~Window()
{
	SAFE_DELETE(m_pTitle);
	SAFE_DELETE(m_pClassName);
}


void Window::CreateWindowFrame(HINSTANCE hInstance)
{
	m_WindowHandle = CreateWindow(m_pClassName, m_pTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (m_WindowHandle == NULL)
		throw std::exception("CreateWindow Failed");

	ShowWindow(m_WindowHandle, SW_SHOW);
	UpdateWindow(m_WindowHandle);
}


void Window::RegisterWindowClass(HINSTANCE hInstance, WNDPROC lpfnWndProc)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = lpfnWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = m_pClassName;
	wcex.hIconSm = NULL;

	if (!RegisterClassEx(&wcex))
		throw std::exception("RegisterClassEx Failed");
}


long Window::GetClientWidth()
{
	RECT lpRect;

	if (!GetClientRect(m_WindowHandle, &lpRect))
		throw std::exception("GetClientRect Failed");

	return lpRect.right - lpRect.left;
}


long Window::GetClientHeight()
{
	RECT lpRect;

	if (!GetClientRect(m_WindowHandle, &lpRect))
		throw std::exception("GetClientRect Failed");

	return lpRect.bottom - lpRect.top;
}


float Window::GetAspectRatio()
{
	RECT lpRect;

	if (!GetClientRect(m_WindowHandle, &lpRect))
		throw std::exception("GetClientRect Failed");

	return (float)(lpRect.right - lpRect.left) / (lpRect.bottom - lpRect.top);
}
