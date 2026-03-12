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

	void OnCollisionEvent(const SabadEngine::Core::Event& e);
	void OnFireworkEvent(const SabadEngine::Core::Event& e);

	// Camera and Lighting
	SabadEngine::Graphics::Camera mCamera;
	SabadEngine::Graphics::DirectionalLight mDirectionalLight;

	// Rendering
	SabadEngine::Graphics::StandardEffect mStandardEffect;

	// Character 1 - Animated character model
	SabadEngine::Graphics::RenderGroup mCharacter01;
	SabadEngine::Graphics::Animator mChar01Animator;
	SabadEngine::Graphics::Animation mChar01Movement;
	float mChar01MoveTime = 0.0f;

	// Character 2 - Animated character model
	SabadEngine::Graphics::RenderGroup mCharacter02;
	SabadEngine::Graphics::Animator mChar02Animator;
	SabadEngine::Graphics::Animation mChar02Movement;
	float mChar02MoveTime = 0.0f;

	// Particle Systems
	SabadEngine::Graphics::ParticleSystemEffect mParticleEffect;
	SabadEngine::Physics::ParticleSystem mChar01Bullets;
	SabadEngine::Physics::ParticleSystem mChar02Bullets;
	SabadEngine::Physics::ParticleSystem mExplosionParticles;

	// Audio
	SabadEngine::Audio::SoundId mBgMusicId = 0;
	SabadEngine::Audio::SoundId mWarningSoundId = 0;
	SabadEngine::Audio::SoundId mExplosionSoundId = 0;
	SabadEngine::Audio::SoundId mFireworkSoundId = 0;

	// Events
	SabadEngine::Core::EventListenerId mCollisionListenerId = 0;
	SabadEngine::Core::EventListenerId mFireworkListenerId = 0;

	// Scene Timer
	float mSceneTime = 0.0f;
	float mTotalSceneTime = 50.0f;
	bool mBgMusicStarted = false;
};