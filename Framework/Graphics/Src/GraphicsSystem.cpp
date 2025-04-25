#include "Precompiled.h"
#include "GraphicsSystem.h"

using namespace SabadEngine;
using namespace SabadEngine::Graphics;
namespace {
	std::unique_ptr<GraphicsSystem> sGraphicsSystem;
	Core::WindowMessageHandler sWindowMessageHandler;
}

LRESULT GraphicsSystem::GraphicsSystemMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (sGraphicsSystem != nullptr) {
		switch (message) {
		case WM_SIZE: {
			const uint32_t width = static_cast<uint32_t>(LOWORD(lParam));
			const uint32_t height = static_cast<uint32_t>(HIWORD(lParam));
			sGraphicsSystem->Resize(width, height);
			break;
		}
		}
	}
	return sWindowMessageHandler.ForwardMessage(window, message, wParam, lParam);
}

void GraphicsSystem::StaticInitialize(HWND window, bool fullscreen)
{
	ASSERT(sGraphicsSystem == nullptr, "GraphicsSystem already initialized!");
	sGraphicsSystem = std::make_unique<GraphicsSystem>();
	sGraphicsSystem->Initialize(window, fullscreen);
}

void GraphicsSystem::StaticTerminate()
{
	if (sGraphicsSystem != nullptr)
	{
		sGraphicsSystem->Terminate();
		sGraphicsSystem.reset();
	}
}

GraphicsSystem* GraphicsSystem::Get()
{
	ASSERT(sGraphicsSystem != nullptr, "GraphicsSystem not initialized!");
	return sGraphicsSystem.get();
}
GraphicsSystem::~GraphicsSystem()
{
	ASSERT(mD3DDevice == nullptr, "GraphicsSystem not terminated!");
}

void GraphicsSystem::Initialize(HWND window, bool fullscreen)
{
	RECT clientRect = {};
	GetClientRect(window, &clientRect);
	UINT width = (UINT)(clientRect.right - clientRect.left);
	UINT height = (UINT)(clientRect.bottom - clientRect.top);

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = window;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = !fullscreen;

	const D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_1;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		&featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&mSwapChain,
		&mD3DDevice,
		nullptr,
		&mImmediateContext);
	ASSERT(SUCCEEDED(hr), "Failed to create device or swap chain!");
	mSwapChain->GetDesc(&mSwapChainDesc);
	Resize(GetBackBufferWidth(), GetBackBufferHeight());
	sWindowMessageHandler.Hook(window, GraphicsSystemMessageHandler);

}

void GraphicsSystem :: Terminate() {
	sWindowMessageHandler.Unhook();
	SafeRelease(mRenderTargetView);
	SafeRelease(mDepthStencilBuffer);
	SafeRelease(mRenderTargetView);
	SafeRelease(mSwapChain);
	SafeRelease(mImmediateContext);
	SafeRelease(mD3DDevice);
}

void GraphicsSystem::BeginRender()
{
	mImmediateContext-> - MsetRenderTarets(1, &mRenderTargetView, mDepthStencilView);
	mImmediateContext->ClearRenderTargetView(mRenderTargetView, (FLOAT*)(*mClearColor));
	mImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
void GraphicsSystem::Resize(uint32_t width, uint32_t height) {
	mImmediateContext->OMSetRenderTargets(0, nullptr, nullptr);
	SafeRelease(mRenderTargetView);
	SafeRelease(mDepthStencilView);
	SafeRelease(mDepthStencilBuffer);

	HRESULT hr;
	if (width != GetBackBufferWidth() || height != GetBackBufferHeight()) {
		hr = mSwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
		ASSERT(SUCCEEDED(hr), "Failed to resize swap chain buffers!");
	}
	else {
		hr = mSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
		ASSERT(SUCCEEDED(hr), "Failed to resize swap chain buffers!");
	}
	ID3D11Texture2D* backBuffer = nullptr;
	hr = mSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	ASSERT(SUCCEEDED(hr), "Failed to get back buffer!");
	hr = mD3DDEVICE->CreateRenderTargetView(backBuffer, nullptr, &mRenderTargetView);
	SafeRelease(backBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create render target view!");
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthDesc.Width = GetBackBufferWidth);
	depthStencilDesc.Height = GetBackBufferHeight();
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	hr = mD3DDevice->CreateTexture2D(&depthStencilDesc, nullptr, &mDepthStencilBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create depth stencil buffer!");
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc= {};
	dsvDesc.Format = depthDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	hr = mD3DDevice->CreateDepthStencilView(mDepthStencilBuffer, &dsvDesc, &mDepthStencilView);
	ASSERT(SUCCEEDED(hr), "Failed to create depth stencil view!");
	ResetRenderTarget();

	mViewport.Width = static_cast<float>(GetBackBufferWidth());
	mViewport.Height = static_cast<float>(GetBackBufferHeight());
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;
	mViewport.TopLeftX = 0.0f;
	mViewport.TopLeftY = 0.0f;
	ResetViewport();
}





void GraphicsSystem::SetClearColor(const Color&)
{

}












