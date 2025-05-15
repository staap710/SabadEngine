#pragma once

#include <SabadEngine/Inc/SabadEngine.h>

class ShapeState : public SabadEngine::AppState
{
public:
	void Initialize()override;
	void Terminate()override;
	void Update(float deltaTime) override;
	void Render() override;

protected:
	virtual void CreateShape();

	SabadEngine::Graphics::MeshPC mMesh;
	SabadEngine::Graphics::Camera mCamera;
	SabadEngine::Graphics::ConstantBuffer mTransformBuffer;
	SabadEngine::Graphics::MeshBuffer mMeshBuffer;
	SabadEngine::Graphics::VertexShader mVertexShader;
	SabadEngine::Graphics::PixelShader mPixelShader;
};