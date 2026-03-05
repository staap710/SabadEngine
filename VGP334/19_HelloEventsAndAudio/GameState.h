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

	void OnSpacePressedEvent(const SabadEngine::Core::Event& e);

	SabadEngine::Graphics::Camera mCamera;
	SabadEngine::Graphics::ParticleSystemEffect mParticleSystemEffect;
	SabadEngine::Physics::ParticleSystem mParticleSystem;
	SabadEngine::Physics::ParticleSystem mFireworkParticles;

	SabadEngine::Core::EventListenerId mSpacePressedListenerId = 0;
	SabadEngine::Core::EventListenerId mEnterPressedListenerId = 0;

	SabadEngine::Graphics::Animation mFireworkAnimation;
	float mFireworkAnimationTime = 0.0f;

	SabadEngine::Audio::SoundId mLaunchSoundId = 0;
	SabadEngine::Audio::SoundId mExplosionSoundId = 0;

};