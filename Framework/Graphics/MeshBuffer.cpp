
#include"Precompiled.h"
#include "MeshBuffer.h"
#include "GraphicsSystem.h"
using namespace SabadEngine;
using namespace SabadEngine::Graphics;

void SabadEngine::Graphics::MeshBuffer::Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount)
{
	auto device = GraphicsSystem::Get()->GetDevice();
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = static_cast<UINT>(vertexCount) * vertexSize;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.MiscFlags = 0;
}

void SabadEngine::Graphics::MeshBuffer::Terminate()
{
}

void SabadEngine::Graphics::MeshBuffer::SetTopology(Topology topology)
{
}

void SabadEngine::Graphics::MeshBuffer::Render() const
{
}

void SabadEngine::Graphics::MeshBuffer::CreateVertexBuffer(const void* vertices, uint32_t vertexSize, uint32_t vertexCount)
{
}
