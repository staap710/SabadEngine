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

	SabadEngine::Graphics::RenderGroup character01;
	SabadEngine::Graphics::RenderGroup character02;
	SabadEngine::Graphics::RenderGroup character03;

	SabadEngine::Graphics::Animator mCharacter01Animator;
	SabadEngine::Graphics::Animator mCharacter02Animator;
	SabadEngine::Graphics::Animator mCharacter03Animator;

	SabadEngine::Graphics::StandardEffect mStandardEffect;

	int mClipIndex = -1;
	float mAnimationSpeed = 1.0f;
	bool mDrawSkeleton = false;
};