#include "Texture.h"
#include <DirectXTK/Inc/WICTextureLoader.h>
#include "GraphicsSystem.h"

SabadEngine::Graphics::Texture::~Texture()
{ 
	ASSERT(mShaderResourceView != nullptr, "Texture not initialized");
}

SabadEngine::Graphics::Texture::Texture(Texture&& rhs) noexcept
{
	mShaderResourceView = rhs.mShaderResourceView;
	rhs.mShaderResourceView = nullptr;
	return *this;
}

Texture& SabadEngine::Graphics::Texture::operator=(Texture&& rhs) noexcept
{
	// TODO: insert return statement here
}

void SabadEngine::Graphics::Texture::Initialize(const std::filesystem::path& fileName)
{
	auto device = GraphicsSystem::Get()->GetDevice();
	auto context = GraphicsSystem::Get()->GetContext();
	// Load the texture from file
	HRESULT hr = DirectX::CreateWICTextureFromFile(device, context, fileName.c_str(),nullptr,&mShaderResourceView);

	ASSERT(SUCCEEDED(hr), "Failed to create texture from file");
}

void SabadEngine::Graphics::Texture::Terminate()
{
	SafeRelease(mShaderResourceView);
}

void SabadEngine::Graphics::Texture::BindVS(uint32_t slot) const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->PSSetShaderResources(slot, 1, &mShaderResourceView);
}

void SabadEngine::Graphics::Texture::BindPS(uint32_t slot) const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->PSSetShaderResources(slot, 1, &mShaderResourceView);
}

void* SabadEngine::Graphics::Texture::GetRawData() const
{
	return mShaderResourceView;
}
