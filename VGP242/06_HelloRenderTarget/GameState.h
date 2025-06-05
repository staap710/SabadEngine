#pragma once

#include <SabadEngine/Inc/SabadEngine.h>

class GameState : public SabadEngine::AppState
{
public:
	void Initialize() override;

	void Terminate() override;

	void Update(float deltaTime) override;

	void Render() override;

	void DebugUI() override;

private:
	struct Object
	{
		SabadEngine::Math::Matrix4 worldMat = SabadEngine::Math::Matrix4::Identity;
		SabadEngine::Graphics::MeshBuffer meshBuffer;
		SabadEngine::Graphics::TextureId textureId = 0;
	};

	void UpdateCamera(float deltaTime);

	void RenderObject(const Object& object, const SabadEngine::Graphics::Camera& camera);

	SabadEngine::Graphics::Camera mCamera;
	SabadEngine::Graphics::Camera mRenderTargetCamera;

	// GPU Communication
	SabadEngine::Graphics::ConstantBuffer mTransformBuffer;
	SabadEngine::Graphics::VertexShader mVertexShader;
	SabadEngine::Graphics::PixelShader mPixelShader;
	SabadEngine::Graphics::Sampler mSampler;

	// Render Object
	Object mSpace;
	Object mEarth;
	Object mSun;

	// Render Target
	SabadEngine::Graphics::RenderTarget mRenderTarget;
};