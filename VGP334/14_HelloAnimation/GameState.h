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

	SabadEngine::Graphics::RenderObject mRenderObject_Earth;
	SabadEngine::Graphics::RenderObject mRenderObject_Metal;
	SabadEngine::Graphics::RenderObject mRenderObject_Wood;
	SabadEngine::Graphics::RenderObject mRenderObject_Water;

	SabadEngine::Graphics::StandardEffect mStandardEffect;

	SabadEngine::Graphics::Animation mAnimation;
	float mAnimationTime = 0.0f;
};