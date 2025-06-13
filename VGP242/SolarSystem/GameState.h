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
	struct SpaceObject
	{
		float radius;
		SabadEngine::Math::Matrix4 centerObject;
		float distanceFromCenter;
		float orbitTranslation;
		float rotationOnAxis;
		float orbitAngle = 0.0f;
		float selfRotationAngle = 0.0f;
		SabadEngine::Graphics::SimpleTextureEffect::RenderData renderData;
	};

	void UpdateCamera(float deltaTime);
	void UpdateRenderTargetCamera(float deltaTime, Math::Vector3 targetPosition, float objectRadius);
	void UpdatePlanet(SpaceObject& planet, float deltaTime);
	void UpdateMoon(SpaceObject& planet, const SpaceObject& earth, float deltaTime);

	SabadEngine::Graphics::Camera mCamera;
	SabadEngine::Graphics::Camera mRenderTargetCamera;

	//GPU communication
	SabadEngine::Graphics::SimpleTextureEffect mSimpleTextureEffect;

	//render main objects
	SabadEngine::Graphics::SimpleTextureEffect::RenderData space;
	SabadEngine::Graphics::SimpleTextureEffect::RenderData sun;

	//planets and the moon
	SpaceObject mercury;
	SpaceObject venus;
	SpaceObject earth;
	SpaceObject moon;
	SpaceObject mars;
	SpaceObject jupiter;
	SpaceObject saturn;
	SpaceObject uranus;
	SpaceObject neptune;
	SpaceObject pluto;

	//render target
	SabadEngine::Graphics::RenderTarget mRenderTarget;

	bool orbitRings = true;
};