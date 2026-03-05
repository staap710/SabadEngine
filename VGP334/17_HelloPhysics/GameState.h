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
	SabadEngine::Graphics::StandardEffect mStandardEffect;

	SabadEngine::Graphics::RenderObject mGroundObject;
	SabadEngine::Physics::CollisionShape mGroundShape;
	SabadEngine::Physics::RigidBody mGroundRigidBody;

	struct Segment
	{
		SabadEngine::Graphics::RenderObject segment;
		SabadEngine::Physics::CollisionShape segmentShape;
		SabadEngine::Physics::RigidBody segmentRB;
		float range = 0.0f;
	};
	std::vector<Segment> mSegments;
	SabadEngine::Physics::CollisionShape mBodyAnchorShape;
	SabadEngine::Physics::RigidBody mBodyAnchor;
};