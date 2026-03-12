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
}

void GameState::Initialize()
{
	// Camera
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

	// ============================================================
	// CHARACTER 01 - Animated 3D Model (left side, faces right)
	// ============================================================
	ModelManager* mm = ModelManager::Get();
	mCharacter01.Initialize("Character01/Character01.model");
	mCharacter01.transform.position = { -4.0f, 0.0f, 0.0f };
	mCharacter01.animator = &mChar01Animator;

	// Add animations for Character 01
	mm->AddAnimation(mCharacter01.modelId, L"../../Assets/Models/Character01/Capoeira.animset");
	mm->AddAnimation(mCharacter01.modelId, L"../../Assets/Models/Character01/RobotHipHopDance.animset");
	mm->AddAnimation(mCharacter01.modelId, L"../../Assets/Models/Character01/Dying.animset");

	mChar01Animator.Initialize(mCharacter01.modelId);
	mChar01Animator.PlayAnimation(0, true); // Start with Capoeira fighting animation

	// ============================================================
	// CHARACTER 02 - Animated 3D Model (right side, faces left)
	// ============================================================
	mCharacter02.Initialize("Character02/Character02.model");
	mCharacter02.transform.position = { 4.0f, 0.0f, 0.0f };
	mCharacter02.transform.rotation = Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, Math::Constants::Pi);
	mCharacter02.animator = &mChar02Animator;

	// Character02 shares the same skeleton, so we can add the same animations
	mm->AddAnimation(mCharacter02.modelId, L"../../Assets/Models/Character01/Capoeira.animset");
	mm->AddAnimation(mCharacter02.modelId, L"../../Assets/Models/Character01/RobotHipHopDance.animset");
	mm->AddAnimation(mCharacter02.modelId, L"../../Assets/Models/Character01/Dying.animset");

	mChar02Animator.Initialize(mCharacter02.modelId);
	mChar02Animator.PlayAnimation(1, true); // Start with HipHop dance animation

	// ============================================================
	// PARTICLE EFFECTS
	// ============================================================
	mParticleEffect.Initialize();
	mParticleEffect.SetCamera(mCamera);
	TextureManager* tm = TextureManager::Get();

	// ---- CHARACTER 01 BULLETS: Blue projectiles firing toward Char02 (+X direction) ----
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

	// ---- CHARACTER 02 BULLETS: Red projectiles firing toward Char01 (-X direction) ----
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
	// CHARACTER 01 MOVEMENT ANIMATION - 50 SECONDS
	// Left side fighter: approach, clash, get knocked back
	// ============================================================
	auto playWarningSound = [&]()
		{
			SoundEffectManager::Get()->Play(mWarningSoundId);
		};
	auto triggerCollision = [&]()
		{
			mExplosionParticles.SetPosition({ 0.0f, 1.0f, 0.0f });
			mExplosionParticles.SpawnParticles();
			SoundEffectManager::Get()->Play(mExplosionSoundId);
			CollisionEvent evt;
			EventManager::Broadcast(evt);
		};
	auto triggerSecondExplosion = [&]()
		{
			mExplosionParticles.SetPosition({ 0.0f, 1.5f, 0.0f });
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
	auto switchChar01ToDying = [&]()
		{
			mChar01Animator.PlayAnimation(2, false); // Play dying animation
		};
	auto switchChar02ToDance = [&]()
		{
			mChar02Animator.PlayAnimation(1, true); // Victory dance
		};

	mChar01Movement = AnimationBuilder()
		// ---- POSITION KEYS ----
		// Act 1: Stand ground / fighting stance at left (0-15s)
		.AddPositionKey({ -4.0f, 0.0f, 0.0f }, 0.0f)
		.AddPositionKey({ -3.8f, 0.0f, 0.2f }, 3.0f)
		.AddPositionKey({ -3.5f, 0.0f, -0.2f }, 6.0f)
		.AddPositionKey({ -3.8f, 0.0f, 0.1f }, 9.0f)
		.AddPositionKey({ -3.5f, 0.0f, 0.0f }, 12.0f)
		.AddPositionKey({ -3.0f, 0.0f, 0.0f }, 15.0f)
		// Act 2: Advance toward opponent (15-30s)
		.AddPositionKey({ -2.5f, 0.0f, 0.0f }, 18.0f)
		.AddPositionKey({ -2.0f, 0.0f, 0.0f }, 21.0f)
		.AddPositionKey({ -1.5f, 0.0f, 0.0f }, 24.0f)
		.AddPositionKey({ -1.0f, 0.0f, 0.0f }, 27.0f)
		.AddPositionKey({ -0.5f, 0.0f, 0.0f }, 30.0f)
		// Act 3: Clash & get knocked back (30-40s)
		.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 32.0f)    // CLASH!
		.AddPositionKey({ -2.0f, 0.3f, 0.0f }, 34.0f)    // Knocked back
		.AddPositionKey({ -3.0f, 0.0f, 0.0f }, 36.0f)    // Land
		.AddPositionKey({ -3.5f, 0.0f, 0.0f }, 38.0f)    // Stagger
		.AddPositionKey({ -4.0f, 0.0f, 0.0f }, 40.0f)    // Fall down
		// Act 4: Down / aftermath (40-50s)
		.AddPositionKey({ -4.0f, 0.0f, 0.0f }, 43.0f)
		.AddPositionKey({ -4.0f, 0.0f, 0.0f }, 46.0f)
		.AddPositionKey({ -4.0f, 0.0f, 0.0f }, 50.0f)

		// ---- ROTATION KEYS (facing right toward opponent) ----
		.AddRotationKey(Math::Quaternion::Identity, 0.0f)
		.AddRotationKey(Math::Quaternion::Identity, 32.0f)
		// Spin on impact
		.AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, Math::Constants::Pi * 2.0f), 34.0f)
		.AddRotationKey(Math::Quaternion::Identity, 36.0f)
		.AddRotationKey(Math::Quaternion::Identity, 50.0f)

		// ---- SCALE KEYS ----
		.AddScaleKey(Math::Vector3(1.0f, 1.0f, 1.0f), 0.0f)
		.AddScaleKey(Math::Vector3(1.0f, 1.0f, 1.0f), 31.0f)
		// Squash at clash
		.AddScaleKey(Math::Vector3(1.3f, 0.7f, 1.3f), 32.0f)
		.AddScaleKey(Math::Vector3(0.8f, 1.3f, 0.8f), 33.0f)
		.AddScaleKey(Math::Vector3(1.0f, 1.0f, 1.0f), 35.0f)
		.AddScaleKey(Math::Vector3(1.0f, 1.0f, 1.0f), 50.0f)

		// ---- EVENT KEYS ----
		.AddEventKey(playWarningSound, 20.0f)         // Warning shot
		.AddEventKey(playWarningSound, 25.0f)         // Another warning
		.AddEventKey(triggerCollision, 32.0f)          // BIG CLASH!
		.AddEventKey(switchChar01ToDying, 36.0f)       // Char01 goes down
		.AddEventKey(triggerSecondExplosion, 35.0f)    // Second explosion
		.AddEventKey(triggerFirework, 45.0f)           // Celebratory firework
		.Build();

	// ============================================================
	// CHARACTER 02 MOVEMENT ANIMATION - 50 SECONDS
	// Right side fighter: approach, clash, victorious
	// ============================================================
	mChar02Movement = AnimationBuilder()
		// ---- POSITION KEYS ----
		// Act 1: Stand ground at right (0-15s)
		.AddPositionKey({ 4.0f, 0.0f, 0.0f }, 0.0f)
		.AddPositionKey({ 3.8f, 0.0f, -0.2f }, 3.0f)
		.AddPositionKey({ 3.5f, 0.0f, 0.2f }, 6.0f)
		.AddPositionKey({ 3.8f, 0.0f, -0.1f }, 9.0f)
		.AddPositionKey({ 3.5f, 0.0f, 0.0f }, 12.0f)
		.AddPositionKey({ 3.0f, 0.0f, 0.0f }, 15.0f)
		// Act 2: Advance toward opponent (15-30s)
		.AddPositionKey({ 2.5f, 0.0f, 0.0f }, 18.0f)
		.AddPositionKey({ 2.0f, 0.0f, 0.0f }, 21.0f)
		.AddPositionKey({ 1.5f, 0.0f, 0.0f }, 24.0f)
		.AddPositionKey({ 1.0f, 0.0f, 0.0f }, 27.0f)
		.AddPositionKey({ 0.5f, 0.0f, 0.0f }, 30.0f)
		// Act 3: Clash & push forward (30-40s)
		.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 32.0f)    // CLASH!
		.AddPositionKey({ -0.5f, 0.2f, 0.0f }, 34.0f)    // Push through
		.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 36.0f)     // Return to center
		.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 38.0f)     // Stand victorious
		.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 40.0f)
		// Act 4: Victory celebration (40-50s)
		.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 43.0f)
		.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 46.0f)
		.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 50.0f)

		// ---- ROTATION KEYS (facing left toward opponent) ----
		.AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, Math::Constants::Pi), 0.0f)
		.AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, Math::Constants::Pi), 32.0f)
		// Victory spin
		.AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, Math::Constants::Pi * 3.0f), 34.0f)
		.AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, Math::Constants::Pi), 36.0f)
		.AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, Math::Constants::Pi), 50.0f)

		// ---- SCALE KEYS ----
		.AddScaleKey(Math::Vector3(1.0f, 1.0f, 1.0f), 0.0f)
		.AddScaleKey(Math::Vector3(1.0f, 1.0f, 1.0f), 31.0f)
		// Squash at clash
		.AddScaleKey(Math::Vector3(1.2f, 0.8f, 1.2f), 32.0f)
		.AddScaleKey(Math::Vector3(0.9f, 1.2f, 0.9f), 33.0f)
		.AddScaleKey(Math::Vector3(1.0f, 1.0f, 1.0f), 35.0f)
		// Grow slightly for victory
		.AddScaleKey(Math::Vector3(1.1f, 1.1f, 1.1f), 42.0f)
		.AddScaleKey(Math::Vector3(1.0f, 1.0f, 1.0f), 50.0f)

		// ---- EVENT KEYS ----
		.AddEventKey(switchChar02ToDance, 38.0f)       // Victory dance!
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
		}
	}

	if (mChar02Movement.GetDuration() > 0)
	{
		mChar02Movement.PlayEvents(prevChar02Time, mChar02MoveTime);
		while (mChar02MoveTime >= mChar02Movement.GetDuration())
		{
			mChar02MoveTime -= mChar02Movement.GetDuration();
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

	// Render the two characters with standard effect
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
	ImGui::Begin("Space Battle - Animation Scene", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	// ---- TIMER DISPLAY ----
	ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "SCENE TIMER");
	ImGui::Separator();

	float displayTime = fmod(mSceneTime, mTotalSceneTime);
	int minutes = static_cast<int>(mSceneTime) / 60;
	int seconds = static_cast<int>(mSceneTime) % 60;

	ImGui::Text("Elapsed: %02d:%02d (%.1fs total)", minutes, seconds, mSceneTime);
	ImGui::Text("Scene Duration: %.0fs", mTotalSceneTime);
	ImGui::Text("Current Loop Time: %.1fs / %.0fs", displayTime, mTotalSceneTime);

	// Progress bar for current loop
	float progress = displayTime / mTotalSceneTime;
	ImGui::ProgressBar(progress, ImVec2(300, 20));

	// Scene phase indicator
	ImGui::Separator();
	ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "SCENE PHASE");
	if (displayTime < 15.0f)
		ImGui::Text("Act 1: Standoff (fighters sizing each other up)");
	else if (displayTime < 30.0f)
		ImGui::Text("Act 2: Advance (closing in, bullets flying!)");
	else if (displayTime < 40.0f)
		ImGui::Text("Act 3: CLASH! (impact & knockback)");
	else
		ImGui::Text("Act 4: Aftermath (victory celebration)");

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