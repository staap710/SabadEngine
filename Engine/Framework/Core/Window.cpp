#include "Window.h"

void SpEngine::Core::Window::Initialize(HINSTANCE instance, const std::wstring& appName, uit32_t width, uit32_t height)
{
}

void SpEngine::Core::Window::Terminate()
{
}

void SpEngine::Core::Window::ProcessMessage()
{
}

HWND SpEngine::Core::Window::GetWindowHandle() const
{
	return HWND();
}

bool SpEngine::Core::Window::IsActive() const
{
	return false;
}
