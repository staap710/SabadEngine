#pragma once

#include <SabadEngine/Inc/SabadEngine.h>

class ShapeState : public SabadEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;

protected:
	virtual void CreateShape();

	using Vertices = std::vector<SabadEngine::Graphics::VertexPC>;
	Vertices mVertices;

	SabadEngine::Graphics::MeshBuffer mMeshbuffer;
	SabadEngine::Graphics::VertexShader mVertexShader;
	SabadEngine::Graphics::PixelShader mPixelShader;
};