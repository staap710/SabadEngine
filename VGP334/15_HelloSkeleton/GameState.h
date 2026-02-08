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
	
	SabadEngine::Graphics::StandardEffect mStandardEffect;

	bool mDrawSkeleton = false;
};