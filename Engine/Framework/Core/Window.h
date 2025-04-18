#pragma once

namespace SpEngine::Core
{
	class Window
	{
	public:
		void Initialize(HINSTANCE instance, const std::wstring& appName, uit32_t width, uit32_t height);
		void Terminate();
		void ProcessMessage();
		HWND GetWindowHandle() const;
		bool IsActive() const;
	private:
		HINSTANCE mInstance = nullptr;
		HWND mWindow = nullptr;
		RECT mScreenRect{};

	};
}