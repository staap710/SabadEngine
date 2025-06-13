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
	SabadEngine::Graphics::Camera mRenderTargetCamera;

	//GPU communication
	SabadEngine::Graphics::SimpleTextureEffect mSimpleTextureEffect;

	//render object
	SabadEngine::Graphics::SimpleTextureEffect::RenderData mObject0;
	SabadEngine::Graphics::SimpleTextureEffect::RenderData mObject1;

	//render target
	SabadEngine::Graphics::RenderTarget mRenderTarget;
};