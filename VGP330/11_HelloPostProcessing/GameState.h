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

	void UpdateCamera(float deltaTime);

	SabadEngine::Graphics::Camera mCamera;
	SabadEngine::Graphics::DirectionalLight mDirectionalLight;

	SabadEngine::Graphics::RenderGroup mCharacter;
	SabadEngine::Graphics::RenderObject mGround;
	SabadEngine::Graphics::RenderObject mSphere;
	SabadEngine::Graphics::StandardEffect mStandardEffect;
	SabadEngine::Graphics::PostProcessingEffect mPostProcessingEffect;
	SabadEngine::Graphics::RenderTarget mRenderTarget;
	SabadEngine::Graphics::RenderObject mScreenQuad;
	
};