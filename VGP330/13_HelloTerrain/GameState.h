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
	SabadEngine::Graphics::RenderGroup mCharacter2; 
	SabadEngine::Graphics::RenderGroup mCharacter3;
	SabadEngine::Graphics::RenderObject mGround;
	SabadEngine::Graphics::RenderObject mSphere;

	SabadEngine::Graphics::Terrain mTerrain;

	SabadEngine::Graphics::RenderObject mScreenQuad;
	SabadEngine::Graphics::StandardEffect mStandardEffect;
	SabadEngine::Graphics::ShadowEffect mShadowEffect;
	SabadEngine::Graphics::TerrainEffect mTerrainEffect;
	
};