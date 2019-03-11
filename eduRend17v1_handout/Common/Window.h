#pragma once

class Window
{
public:
	Window(const char Title[], const char ClassName[], HINSTANCE hInstance, WNDPROC lpfnWndProc);
	virtual ~Window();

	void CreateWindowFrame(HINSTANCE hInstance);
	void RegisterWindowClass(HINSTANCE hInstance, WNDPROC lpfnWndProc);
	std::wstring GetCurrentPath();
	long GetClientWidth();
	long GetClientHeight();
	float GetAspectRatio();
	inline const HWND GetWindowHandle();

private:
	char* m_pTitle;
	char* m_pClassName;
	HWND m_WindowHandle;
};


inline const HWND Window::GetWindowHandle()
{
	return m_WindowHandle;
}