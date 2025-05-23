#include "DebugUI.h"
#include "Precompiled.h"
#include "GraphicsSystem.h"

#include <ImGui/Inc/imgui_impl_dx11.h>
#include <ImGui/Inc/imgui_impl_win32.h>

using namespace SabadEngine;
using namespace SabadEngine::Graphics;
using namespace SabadEngine::Core;


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace{
	WindowMessageHandler sWindowMessageHandler;
	bool IsMouseInput(UINT msg) {
		switch (msg) {
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEMOVE:
		case WM_MOUSEWHEEL:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_MOUSELEAVE:
			return true;
		default:
			break;
		}
		return false;
	}
	LRESULT CALLBACK DebugUIMessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		ImGui& io = ImGui::GetIO();
		//does imgui want to capture the muse and is it a mouse message
		if (io.WantCaptureMouse && IsMouseInput(msg))
		{
			return ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
		}


		//does imgui want to capture the keyboard and is it a keyboard message
		if(io.WantCaptureKeyboard)
		{
			return ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
		}
		return sWindowMessageHandler.ForwardMessage(hwnd, msg, wParam, lParam);
	}
}

void SabadEngine::Graphics::DebugUI::StaticInitialize(HWND window, bool docking, bool multiViewport)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext;
	ImGuiIO& io = ImGui::GetIO(); 
	if (docking)
	{
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	}
	if (multiViewport)
	{
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	}
	auto device = GraphicsSystem::Get()->GetDevice();
	auto context = GraphicsSystem::Get()->GetContext();
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(device, context);
	sWindowMessageHandler.Hook(window, DebugUIMessageHandler);

}

void SabadEngine::Graphics::DebugUI::StaticTerminate()
{
	sWindowMessageHandler.Unhook();
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void SabadEngine::Graphics::DebugUI::SetTheme(Theme theme)
{
	switch (theme) {
	case Theme::Classic:{
		ImGui::StyleColorsClassic();
		break;
	}
	case Theme::Dark: {
		ImGui::StyleColorsDark();
		break;
	}
	case Theme::Light: {
		ImGui::StyleColorsLight();
		break;
	}
	default:
		ASSERT(false, "DEBUGUI UNKNOWN THEME");
		break;
}

void SabadEngine::Graphics::DebugUI::BeginRender()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
};

void SabadEngine::Graphics::DebugUI::EndRender()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}
