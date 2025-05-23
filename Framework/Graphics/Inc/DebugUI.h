#pragma once

namespace SabadEngine::Graphics::DebugUI
{
	enum class Theme
	{
		Light,
		Dark,
		Classic,
	};

	void StaticInitialize(HWND window, bool docking= false, bool multiViewport= false);
	void StaticTerminate();
	void SetTheme(Theme theme);
	void BeginRender();
	void EndRender();
}