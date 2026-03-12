#include "GameState.h"
#include "GameEvents.h"

using namespace SabadEngine;
using namespace SabadEngine::Graphics;
using namespace SabadEngine::Input;
using namespace SabadEngine::Physics;
using namespace SabadEngine::Audio;

namespace
{
	const float SCENE_DURATION = 50.0f;

	// Animation clip indices (order they are added via AddAnimation)
	// 0 = Capoeira
	// 1 = WalkingDefensive
	// 2 = FastRun
	// 3 = SneakWalk
	// 4 = StandardWalk
	// 5 = PunchCombo
	// 6 = Punching
	// 7 = RobotHipHopDance
	// 8 = Dying
	enum AnimClip
	{
		Capoeira = 0,
		WalkingDefensive,
		FastRun,
		SneakWalk,
		StandardWalk,
		PunchCombo,
		Punching,
		HipHopDance,
		Dying
	};
}

void GameState::Initialize()
{
	// Camera - positioned to see characters facing each other
	mCamera.SetPosition({ 0.0f, 2.0f, -8.0f });
	mCamera.SetLookAt({ 0.0f, 1.0f, 0.0f });

	// Lighting
	mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.3f, 0.3f, 0.4f, 1.0f };
	mDirectionalLight.diffuse = { 0.9f, 0.9f, 0.8f, 1.0f };
	mDirectionalLight.specular = { 1.0f, 1.0f, 1.0f, 1.0f };

	// Standard Effect
	std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(shaderFile);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);

	ModelManager* mm = ModelManager::Get();

	// ============================================================
	// CHARACTER 01 - Left side fighter (faces RIGHT toward Char02)
	// ============================================================
	mCharacter01.Initialize("Character01/Character01.model");
	mCharacter01.transform.position = { -5.0f, 0.0f, 0.0f };
	// Face right (+X) toward Character02
	mCharacter01.transform.rotation = Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, Math::Constants::HalfPi);
	mCharacter01.animator = &mChar01Animator;

	// Add all animations for Character 01
	mm->AddAnimation(mCharacter01.modelId, L"../../Assets/Models/Character01/Capoeira.animset");          // 0
	mm->AddAnimation(mCharacter01.modelId, L"../../Assets/Models/Character01/WalkingDefensive.animset");   // 1
	mm->AddAnimation(mCharacter01.modelId, L"../../Assets/Models/Character01/FastRun.animset");             // 2
	mm->AddAnimation(mCharacter01.modelId, L"../../Assets/Models/Character01/SneakWalk.animset");           // 3
	mm->AddAnimation(mCharacter01.modelId, L"../../Assets/Models/Character01/StandardWalk.animset");        // 4
	mm->AddAnimation(mCharacter01.modelId, L"../../Assets/Models/Character01/PunchCombo.animset");          // 5
	mm->AddAnimation(mCharacter01.modelId, L"../../Assets/Models/Character01/Punching.animset");            // 6
	mm->AddAnimation(mCharacter01.modelId, L"../../Assets/Models/Character01/RobotHipHopDance.animset");    // 7
	mm->AddAnimation(mCharacter01.modelId, L"../../Assets/Models/Character01/Dying.animset");               // 8

	mChar01Animator.Initialize(mCharacter01.modelId);
	mChar01Animator.PlayAnimation(AnimClip::Capoeira, true); // Start with fighting stance

	// ============================================================
	// CHARACTER 02 - Right side fighter (faces LEFT toward Char01)
	// ============================================================
	mCharacter02.Initialize("Character02/Character02.model");
	mCharacter02.transform.position = { 5.0f, 0.0f, 0.0f };
	// Face left (-X) toward Character01
	mCharacter02.transform.rotation = Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, -Math::Constants::HalfPi);
	mCharacter02.animator = &mChar02Animator;

	// Add same animations for Character 02
	mm->AddAnimation(mCharacter02.modelId, L"../../Assets/Models/Character01/Capoeira.animset");          // 0
	mm->AddAnimation(mCharacter02.modelId, L"../../Assets/Models/Character01/WalkingDefensive.animset");   // 1
	mm->AddAnimation(mCharacter02.modelId, L"../../Assets/Models/Character01/FastRun.animset");             // 2
	mm->AddAnimation(mCharacter02.modelId, L"../../Assets/Models/Character01/SneakWalk.animset");           // 3
	mm->AddAnimation(mCharacter02.modelId, L"../../Assets/Models/Character01/StandardWalk.animset");        // 4
	mm->AddAnimation(mCharacter02.modelId, L"../../Assets/Models/Character01/PunchCombo.animset");          // 5
	mm->AddAnimation(mCharacter02.modelId, L"../../Assets/Models/Character01/Punching.animset");            // 6
	mm->AddAnimation(mCharacter02.modelId, L"../../Assets/Models/Character01/RobotHipHopDance.animset");    // 7
	mm->AddAnimation(mCharacter02.modelId, L"../../Assets/Models/Character01/Dying.animset");               // 8

	mChar02Animator.Initialize(mCharacter02.modelId);
	mChar02Animator.PlayAnimation(AnimClip::Capoeira, true); // Start with fighting stance

	// ============================================================
	// PARTICLE EFFECTS
	// ============================================================
	mParticleEffect.Initialize();
	mParticleEffect.SetCamera(mCamera);
	TextureManager* tm = TextureManager::Get();

	// ---- CHAR01 BULLETS: Blue projectiles firing toward Char02 (+X) ----
	ParticleSystemInfo char01BulletInfo;
	char01BulletInfo.textureId = tm->LoadTexture("Images/bullet2.png");
	char01BulletInfo.maxParticles = 300;
	char01BulletInfo.particlesPerEmit = { 1, 2 };
	char01BulletInfo.delay = 0.0f;
	char01BulletInfo.lifeTime = FLT_MAX;
	char01BulletInfo.timeBetweenEmit = { 0.4f, 0.8f };
	char01BulletInfo.spawnAngle = { -5.0f, 5.0f };
	char01BulletInfo.spawnSpeed = { 8.0f, 12.0f };
	char01BulletInfo.particleLifeTime = { 1.0f, 2.0f };
	char01BulletInfo.spawnDirection = { 1.0f, 0.0f, 0.0f };
	char01BulletInfo.spawnPosition = Math::Vector3::Zero;
	char01BulletInfo.startScale = { Math::Vector3::One * 1.2f, Math::Vector3::One * 1.5f };
	char01BulletInfo.endScale = { Math::Vector3::One * 0.8f, Math::Vector3::One * 1.0f };
	char01BulletInfo.startColour = { Colors::Cyan, Colors::Blue };
	char01BulletInfo.endColour = { Colors::Blue, Colors::LightBlue };
	mChar01Bullets.Initialize(char01BulletInfo);

	// ---- CHAR02 BULLETS: Red projectiles firing toward Char01 (-X) ----
	ParticleSystemInfo char02BulletInfo;
	char02BulletInfo.textureId = tm->LoadTexture("Images/bullet2.png");
	char02BulletInfo.maxParticles = 300;
	char02BulletInfo.particlesPerEmit = { 1, 2 };
	char02BulletInfo.delay = 0.0f;
	char02BulletInfo.lifeTime = FLT_MAX;
	char02BulletInfo.timeBetweenEmit = { 0.4f, 0.8f };
	char02BulletInfo.spawnAngle = { -5.0f, 5.0f };
	char02BulletInfo.spawnSpeed = { 8.0f, 12.0f };
	char02BulletInfo.particleLifeTime = { 1.0f, 2.0f };
	char02BulletInfo.spawnDirection = { -1.0f, 0.0f, 0.0f };
	char02BulletInfo.spawnPosition = Math::Vector3::Zero;
	char02BulletInfo.startScale = { Math::Vector3::One * 1.2f, Math::Vector3::One * 1.5f };
	char02BulletInfo.endScale = { Math::Vector3::One * 0.8f, Math::Vector3::One * 1.0f };
	char02BulletInfo.startColour = { Colors::Red, Colors::OrangeRed };
	char02BulletInfo.endColour = { Colors::DarkRed, Colors::Red };
	mChar02Bullets.Initialize(char02BulletInfo);

	// Explosion particles - big colorful burst (manual spawn only)
	ParticleSystemInfo explosionInfo;
	explosionInfo.textureId = tm->LoadTexture("Images/bullet2.png");
	explosionInfo.maxParticles = 2000;
	explosionInfo.particlesPerEmit = { 30, 60 };
	explosionInfo.delay = 0.0f;
	explosionInfo.lifeTime = 0.0f;
	explosionInfo.timeBetweenEmit = { 0.0f, 0.0f };
	explosionInfo.spawnAngle = { -180.0f, 180.0f };
	explosionInfo.spawnSpeed = { 5.0f, 15.0f };
	explosionInfo.particleLifeTime = { 0.5f, 2.5f };
	explosionInfo.spawnDirection = Math::Vector3::YAxis;
	explosionInfo.spawnPosition = Math::Vector3::Zero;
	explosionInfo.startScale = { Math::Vector3::One * 0.8f, Math::Vector3::One * 1.5f };
	explosionInfo.endScale = { Math::Vector3::Zero, Math::Vector3::Zero };
	explosionInfo.startColour = { Colors::Yellow, Colors::Red };
	explosionInfo.endColour = { Colors::OrangeRed, Colors::White };
	mExplosionParticles.Initialize(explosionInfo);

	// ============================================================
	// AUDIO
	// ============================================================
	SoundEffectManager* sm = SoundEffectManager::Get();
	mBgMusicId = sm->Load("megamanx_spark_mandrill.wav");
	mWarningSoundId = sm->Load("megamanx_shot.wav");
	mExplosionSoundId = sm->Load("explosion.wav");
	mFireworkSoundId = sm->Load("megamanx_blast.wav");

	// ============================================================
	// EVENTS
	// ============================================================
	EventManager* em = EventManager::Get();
	mCollisionListenerId = em->AddListener(CollisionEvent::StaticGetTypeId(),
		std::bind(&GameState::OnCollisionEvent, this, std::placeholders::_1));
	mFireworkListenerId = em->AddListener(FireworkEvent::StaticGetTypeId(),
		std::bind(&GameState::OnFireworkEvent, this, std::placeholders::_1));

	// ============================================================
	// CHOREOGRAPHY LAMBDAS
	// ============================================================
	auto playWarningSound = [&]()
		{
			SoundEffectManager::Get()->Play(mWarningSoundId);
		};
	auto playExplosionSound = [&]()
		{
			SoundEffectManager::Get()->Play(mExplosionSoundId);
		};

	// --- Animation switches for Char01 ---
	auto char01SneakWalk = [&]()
		{
			mChar01Animator.PlayAnimation(AnimClip::SneakWalk, true);
		};
	auto char01WalkDefensive = [&]()
		{
			mChar01Animator.PlayAnimation(AnimClip::WalkingDefensive, true);
		};
	auto char01FastRun = [&]()
		{
			mChar01Animator.PlayAnimation(AnimClip::FastRun, true);
		};
	auto char01PunchCombo = [&]()
		{
			mChar01Animator.PlayAnimation(AnimClip::PunchCombo, true);
			SoundEffectManager::Get()->Play(mWarningSoundId);
		};
	auto char01Dying = [&]()
		{
			mChar01Animator.PlayAnimation(AnimClip::Dying, false);
		};

	// --- Animation switches for Char02 ---
	auto char02StandardWalk = [&]()
		{
			mChar02Animator.PlayAnimation(AnimClip::StandardWalk, true);
		};
	auto char02FastRun = [&]()
		{
			mChar02Animator.PlayAnimation(AnimClip::FastRun, true);
		};
	auto char02Punching = [&]()
		{
			mChar02Animator.PlayAnimation(AnimClip::Punching, true);
			SoundEffectManager::Get()->Play(mWarningSoundId);
		};
	auto char02VictoryDance = [&]()
		{
			mChar02Animator.PlayAnimation(AnimClip::HipHopDance, true);
		};
	auto char02WalkDefensive = [&]()
		{
			mChar02Animator.PlayAnimation(AnimClip::WalkingDefensive, true);
		};
	auto char02PunchCombo = [&]()
		{
			mChar02Animator.PlayAnimation(AnimClip::PunchCombo, true);
		};

	// --- Collision / explosion events ---
	auto triggerClash = [&]()
		{
			mExplosionParticles.SetPosition({ 0.0f, 1.0f, 0.0f });
			mExplosionParticles.SpawnParticles();
			SoundEffectManager::Get()->Play(mExplosionSoundId);
			CollisionEvent evt;
			EventManager::Broadcast(evt);
		};
	auto triggerSecondExplosion = [&]()
		{
			mExplosionParticles.SetPosition({ -1.0f, 1.5f, 0.0f });
			mExplosionParticles.SpawnParticles();
			SoundEffectManager::Get()->Play(mExplosionSoundId);
		};
	auto triggerFirework = [&]()
		{
			mExplosionParticles.SetPosition({ 0.0f, 4.0f, 0.0f });
			mExplosionParticles.SpawnParticles();
			SoundEffectManager::Get()->Play(mFireworkSoundId);
			FireworkEvent evt;
			EventManager::Broadcast(evt);
		};

	// ============================================================
	// CHARACTER 01 MOVEMENT - 50 SECONDS
	// Choreography:
	//   0-5s:   Capoeira stance (sizing up)
	//   5s:     Switch to SneakWalk -> creep forward
	//   12s:    Switch to WalkingDefensive -> cautious advance
	//   20s:    Switch to FastRun -> charge!
	//   27s:    Switch to PunchCombo -> attack!
	//   32s:    CLASH! Explosion, knocked back
	//   36s:    Dying animation
	//   40-50s: Lying on ground
	// ============================================================
	mChar01Movement = AnimationBuilder()
		// ---- POSITION KEYS ----
		// Act 1: Stance at far left (0-5s)
		.AddPositionKey({ 0.5f, 0.0f, 0.0f }, 0.0f)
		.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 6.0f)
		// Sneak walk forward (5-12s)
		.AddPositionKey({ 0.5f, 0.0f, 0.0f }, 8.0f)
		.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 12.0f)
		// Defensive walk forward (12-20s)
		.AddPositionKey({ 1.0f, 0.0f, 0.0f }, 15.0f)
		.AddPositionKey({ 1.0f, 0.0f, 0.0f }, 18.0f)
		.AddPositionKey({ 1.0f, 0.0f, 0.0f }, 20.0f)
		// Fast run charge (20-27s)
		.AddPositionKey({ 2.0f, 0.0f, 0.0f }, 20.0f)
		.AddPositionKey({ 2.0f, 0.0f, 0.0f }, 25.0f)
		.AddPositionKey({ 2.0f, 0.0f, 0.0f }, 27.0f)
		// Punching approach (27-32s)
		.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 30.0f) // CLASH!
		// Knocked back (32-36s)
		.AddPositionKey({ 0.2f, 0.3f, 0.0f }, 33.5f)
		.AddPositionKey({ 0.5f, 0.0f, 0.0f }, 35.0f)
		.AddPositionKey({ 0.7f, 0.0f, 0.0f }, 36.0f)
		// Lying on ground (36-50s)
		.AddPositionKey({ 1.0f, 0.0f, 0.0f }, 40.0f)
		.AddPositionKey({ 1.0f, 0.0f, 0.0f }, 50.0f)

		// ---- ROTATION KEYS (always facing right toward Char02) ----
		.AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, Math::Constants::HalfPi), 0.0f)
		.AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, Math::Constants::HalfPi), 50.0f)

		// ---- SCALE KEYS ----
		.AddScaleKey(Math::Vector3(1.0f, 1.0f, 1.0f), 0.0f)
		.AddScaleKey(Math::Vector3(1.0f, 1.0f, 1.0f), 31.0f)
		// Squash at clash
		.AddScaleKey(Math::Vector3(1.3f, 0.7f, 1.3f), 32.0f)
		.AddScaleKey(Math::Vector3(0.8f, 1.3f, 0.8f), 33.0f)
		.AddScaleKey(Math::Vector3(1.0f, 1.0f, 1.0f), 35.0f)
		.AddScaleKey(Math::Vector3(1.0f, 1.0f, 1.0f), 50.0f)

		// ---- EVENT KEYS ----
		.AddEventKey(char01SneakWalk, 5.0f)           // Switch to sneak walk
		.AddEventKey(playWarningSound, 5.5f)          // Warning shot
		.AddEventKey(char01PunchCombo, 6.0f)          // Distant explosion
		.AddEventKey(char01WalkDefensive, 12.0f)       // Switch to defensive walk
		.AddEventKey(playWarningSound, 15.0f)          // Warning shot
		.AddEventKey(char01FastRun, 20.0f)             // CHARGE!
		.AddEventKey(playWarningSound, 22.0f)          // Warning shot
		.AddEventKey(char01FastRun, 27.0f)          // Start punching
		.AddEventKey(char01PunchCombo, 30.0f)          // Start punching
		.AddEventKey(triggerClash, 32.0f)              // BIG CLASH!
		.AddEventKey(triggerSecondExplosion, 34.0f)    // Second explosion
		.AddEventKey(char01Dying, 36.0f)               // Char01 goes down
		.AddEventKey(triggerFirework, 45.0f)           // Firework
		.Build();

	// ============================================================
	// CHARACTER 02 MOVEMENT - 50 SECONDS
	// Choreography:
	//   0-8s:   Capoeira stance
	//   8s:     Switch to StandardWalk -> walk toward
	//   18s:    Switch to FastRun -> charge
	//   25s:    Switch to Punching -> attack
	//   32s:    CLASH! Hold ground
	//   38s:    Victory HipHop dance
	//   40-50s: Celebrating
	// ============================================================
	mChar02Movement = AnimationBuilder()
		// ---- POSITION KEYS ----
		// Act 1: Stance at far right (0-8s)
		.AddPositionKey({ -1.0f, 0.0f, 0.0f }, 0.0f)
		.AddPositionKey({ -1.0f, 0.0f, 0.0f }, 8.0f)
		// Standard walk forward (8-18s)
		.AddPositionKey({ -1.0f, 0.0f, 0.0f }, 11.0f)
		.AddPositionKey({ -1.0f, 0.0f, 0.0f }, 14.0f)
		.AddPositionKey({ -1.0f, 0.0f, 0.0f }, 18.0f)
		// Fast run charge (18-25s)
		.AddPositionKey({ -1.0f, 0.0f, 0.0f }, 20.0f)
		.AddPositionKey({ -1.0f, 0.0f, 0.0f }, 22.0f)
		.AddPositionKey({ -1.0f, 0.0f, 0.0f }, 25.0f)
		// Punching approach (25-32s)
		.AddPositionKey({ -1.0f, 0.0f, 0.0f }, 28.0f)
		.AddPositionKey({ -1.0f, 0.0f, 0.0f }, 30.0f)
		.AddPositionKey({ -1.0f, 0.0f, 0.0f }, 32.0f)     // CLASH!
		// Push through (32-38s)
		.AddPositionKey({ -1.0f, 0.0f, 0.0f }, 34.0f)
		.AddPositionKey({ -1.0f, 0.0f, 0.0f }, 36.0f)
		// Victory spot (38-50s)
		.AddPositionKey({ -1.0f, 0.0f, 0.0f }, 38.0f)
		.AddPositionKey({ -1.0f, 0.0f, 0.0f }, 50.0f)

		// ---- ROTATION KEYS (always facing left toward Char01) ----
		.AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, -Math::Constants::HalfPi), 0.0f)
		.AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, -Math::Constants::HalfPi), 50.0f)

		// ---- SCALE KEYS ----
		.AddScaleKey(Math::Vector3(1.0f, 1.0f, 1.0f), 0.0f)
		.AddScaleKey(Math::Vector3(1.0f, 1.0f, 1.0f), 31.0f)
		// Squash at clash
		.AddScaleKey(Math::Vector3(1.2f, 0.8f, 1.2f), 32.0f)
		.AddScaleKey(Math::Vector3(0.9f, 1.2f, 0.9f), 33.0f)
		.AddScaleKey(Math::Vector3(1.0f, 1.0f, 1.0f), 35.0f)
		// Grow for victory
		.AddScaleKey(Math::Vector3(1.1f, 1.1f, 1.1f), 42.0f)
		.AddScaleKey(Math::Vector3(1.0f, 1.0f, 1.0f), 50.0f)

		// ---- EVENT KEYS ----

		.AddEventKey(playWarningSound, 14.0f)          // Warning shot
		.AddEventKey(char02WalkDefensive, 18.0f)             // CHARGE!
		.AddEventKey(char02PunchCombo, 20.0f)
		.AddEventKey(char02Punching, 25.0f)            // Start punching
		.AddEventKey(char02VictoryDance, 38.0f)        // Victory dance!
		.Build();

	// Initialize scene time
	mSceneTime = 0.0f;
	mTotalSceneTime = SCENE_DURATION;
	mBgMusicStarted = false;
}

void GameState::Terminate()
{
	SoundEffectManager* sm = SoundEffectManager::Get();
	sm->Stop(mBgMusicId);
	sm->Stop(mWarningSoundId);
	sm->Stop(mExplosionSoundId);
	sm->Stop(mFireworkSoundId);

	EventManager* em = EventManager::Get();
	em->RemoveListener(CollisionEvent::StaticGetTypeId(), mCollisionListenerId);
	em->RemoveListener(FireworkEvent::StaticGetTypeId(), mFireworkListenerId);

	mExplosionParticles.Terminate();
	mChar02Bullets.Terminate();
	mChar01Bullets.Terminate();
	mParticleEffect.Terminate();

	mCharacter02.Terminate();
	mCharacter01.Terminate();
	mStandardEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
	UpdateCamera(deltaTime);

	// Start background music on first frame
	if (!mBgMusicStarted)
	{
		SoundEffectManager::Get()->Play(mBgMusicId);
		mBgMusicStarted = true;
	}

	// Update skeleton animators
	mChar01Animator.Update(deltaTime);
	mChar02Animator.Update(deltaTime);

	// Track previous time for event playback
	float prevChar01Time = mChar01MoveTime;
	float prevChar02Time = mChar02MoveTime;

	// Advance scene time
	mSceneTime += deltaTime;
	mChar01MoveTime += deltaTime;
	mChar02MoveTime += deltaTime;

	// Play events and loop
	if (mChar01Movement.GetDuration() > 0)
	{
		mChar01Movement.PlayEvents(prevChar01Time, mChar01MoveTime);
		while (mChar01MoveTime >= mChar01Movement.GetDuration())
		{
			mChar01MoveTime -= mChar01Movement.GetDuration();
			// Reset animations on loop
			mChar01Animator.PlayAnimation(AnimClip::Capoeira, true);
		}
	}

	if (mChar02Movement.GetDuration() > 0)
	{
		mChar02Movement.PlayEvents(prevChar02Time, mChar02MoveTime);
		while (mChar02MoveTime >= mChar02Movement.GetDuration())
		{
			mChar02MoveTime -= mChar02Movement.GetDuration();
			// Reset animations on loop
			mChar02Animator.PlayAnimation(AnimClip::Capoeira, true);
		}
	}

	// Update particles
	mChar01Bullets.Update(deltaTime);
	mChar02Bullets.Update(deltaTime);
	mExplosionParticles.Update(deltaTime);
}

void GameState::Render()
{
	// Apply movement animation transforms to characters
	Transform char01Transform = mChar01Movement.GetTransform(mChar01MoveTime);
	mCharacter01.transform.position = char01Transform.position;
	mCharacter01.transform.rotation = char01Transform.rotation;
	mCharacter01.transform.scale = char01Transform.scale;

	Transform char02Transform = mChar02Movement.GetTransform(mChar02MoveTime);
	mCharacter02.transform.position = char02Transform.position;
	mCharacter02.transform.rotation = char02Transform.rotation;
	mCharacter02.transform.scale = char02Transform.scale;

	// Move bullet emitters to character chest height
	Math::Vector3 char01BulletPos = mCharacter01.transform.position + Math::Vector3{ 0.0f, 1.0f, 0.0f };
	Math::Vector3 char02BulletPos = mCharacter02.transform.position + Math::Vector3{ 0.0f, 1.0f, 0.0f };
	mChar01Bullets.SetPosition(char01BulletPos);
	mChar02Bullets.SetPosition(char02BulletPos);

	// Draw ground plane
	SimpleDraw::AddGroundPlane(20.0f, Colors::Wheat);
	SimpleDraw::Render(mCamera);

	// Render the two characters
	mStandardEffect.Begin();
	mStandardEffect.Render(mCharacter01);
	mStandardEffect.Render(mCharacter02);
	mStandardEffect.End();

	// Render all particle systems
	mParticleEffect.Begin();
	mChar01Bullets.Render(mParticleEffect);
	mChar02Bullets.Render(mParticleEffect);
	mExplosionParticles.Render(mParticleEffect);
	mParticleEffect.End();
}

void GameState::DebugUI()
{
	ImGui::Begin("Fighter Showdown - Animation Scene", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	// ---- TIMER DISPLAY ----
	ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "SCENE TIMER");
	ImGui::Separator();

	float displayTime = fmod(mSceneTime, mTotalSceneTime);
	int minutes = static_cast<int>(mSceneTime) / 60;
	int seconds = static_cast<int>(mSceneTime) % 60;

	ImGui::Text("Elapsed: %02d:%02d (%.1fs total)", minutes, seconds, mSceneTime);
	ImGui::Text("Scene Duration: %.0fs", mTotalSceneTime);
	ImGui::Text("Current Loop Time: %.1fs / %.0fs", displayTime, mTotalSceneTime);

	// Progress bar
	float progress = displayTime / mTotalSceneTime;
	ImGui::ProgressBar(progress, ImVec2(300, 20));

	// Scene phase indicator
	ImGui::Separator();
	ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "SCENE PHASE");
	if (displayTime < 5.0f)
		ImGui::Text("Act 1a: Sizing Up (Capoeira stances)");
	else if (displayTime < 12.0f)
		ImGui::Text("Act 1b: Creeping Forward (Sneak Walk)");
	else if (displayTime < 20.0f)
		ImGui::Text("Act 2a: Cautious Advance (Defensive Walk)");
	else if (displayTime < 27.0f)
		ImGui::Text("Act 2b: CHARGE! (Fast Run)");
	else if (displayTime < 32.0f)
		ImGui::Text("Act 3a: ATTACK! (Punch Combo!)");
	else if (displayTime < 38.0f)
		ImGui::Text("Act 3b: CLASH! (Impact & Knockback)");
	else
		ImGui::Text("Act 4: Victory Dance!");

	ImGui::Separator();
	ImGui::TextColored(ImVec4(0.5f, 0.8f, 1.0f, 1.0f), "CHARACTER POSITIONS");
	ImGui::Text("Char01: (%.1f, %.1f, %.1f)", mCharacter01.transform.position.x, mCharacter01.transform.position.y, mCharacter01.transform.position.z);
	ImGui::Text("Char02: (%.1f, %.1f, %.1f)", mCharacter02.transform.position.x, mCharacter02.transform.position.y, mCharacter02.transform.position.z);

	ImGui::Separator();
	if (ImGui::CollapsingHeader("Light Settings"))
	{
		if (ImGui::DragFloat3("Direction##Light", &mDirectionalLight.direction.x, 0.01f))
		{
			mDirectionalLight.direction = Math::Normalize(mDirectionalLight.direction);
		}
		ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
		ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
	}

	if (ImGui::CollapsingHeader("Particle Debug"))
	{
		mChar01Bullets.DebugUI();
	}

	if (ImGui::CollapsingHeader("Physics Debug"))
	{
		PhysicsWorld::Get()->DebugUI();
	}

	mStandardEffect.DebugUI();
	ImGui::End();
}

void GameState::UpdateCamera(float deltaTime)
{
	InputSystem* input = InputSystem::Get();
	const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 4.0f;
	const float turnSpeed = 0.5f;

	if (input->IsKeyDown(KeyCode::W)) mCamera.Walk(moveSpeed * deltaTime);
	else if (input->IsKeyDown(KeyCode::S)) mCamera.Walk(-moveSpeed * deltaTime);

	if (input->IsKeyDown(KeyCode::D)) mCamera.Strafe(moveSpeed * deltaTime);
	else if (input->IsKeyDown(KeyCode::A)) mCamera.Strafe(-moveSpeed * deltaTime);

	if (input->IsKeyDown(KeyCode::E)) mCamera.Rise(moveSpeed * deltaTime);
	else if (input->IsKeyDown(KeyCode::Q)) mCamera.Rise(-moveSpeed * deltaTime);

	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
	}
}

void GameState::OnCollisionEvent(const SabadEngine::Core::Event& e)
{
	LOG("COLLISION EVENT: The fighters have clashed!");
}

void GameState::OnFireworkEvent(const SabadEngine::Core::Event& e)
{
	LOG("FIREWORK EVENT: Victory celebration!");
}