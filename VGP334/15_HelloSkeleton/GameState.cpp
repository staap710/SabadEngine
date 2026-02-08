#include "GameState.h"

using namespace SabadEngine;
using namespace SabadEngine::Graphics;
using namespace SabadEngine::Input;

enum class CurrentModel
{
	Character01,
	Character02,
	Character03
};

const char* gObjectNames[] = { "Character01", "Character02", "Character03" };

CurrentModel gCurrentModel = CurrentModel::Character01;

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.4f, 0.4f, 0.4f, 1.0f };
	mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
	mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

	character01.Initialize("Character01/Character01.model");
	character01.transform.position = { 0.0f, 0.0f, 0.0f };

	character02.Initialize("Character02/Character02.model");
	character02.transform.position = { 0.0f, 0.0f, 0.0f };

	character03.Initialize("Character03/Character03.model");
	character03.transform.position = { 0.0f, 0.0f, 0.0f };

	std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(shaderFile);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
}

void GameState::Terminate()
{
	character01.Terminate();
	character02.Terminate();
	character03.Terminate();
	mStandardEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
}

void GameState::Render()
{
	SimpleDraw::AddGroundPlane(20.0f, mDrawSkeleton ? Colors::DarkRed : Colors::White);
	SimpleDraw::Render(mCamera);

	if (mDrawSkeleton)
	{
		AnimationUtil::BoneTransforms boneTransforms;

		switch (gCurrentModel)
		{
		case CurrentModel::Character01:
			AnimationUtil::ComputeBoneTransforms(character01.modelId, boneTransforms);
			AnimationUtil::DrawSkeleton(character01.modelId, boneTransforms);
			break;
		case CurrentModel::Character02:
			AnimationUtil::ComputeBoneTransforms(character02.modelId, boneTransforms);
			AnimationUtil::DrawSkeleton(character02.modelId, boneTransforms);
			break;
		case CurrentModel::Character03:
			AnimationUtil::ComputeBoneTransforms(character03.modelId, boneTransforms);
			AnimationUtil::DrawSkeleton(character03.modelId, boneTransforms);
			break;
		}
	}
	else
	{
		mStandardEffect.Begin();

		switch (gCurrentModel)
		{
		case CurrentModel::Character01:
			mStandardEffect.Render(character01);
			break;
		case CurrentModel::Character02:
			mStandardEffect.Render(character02);
			break;
		case CurrentModel::Character03:
			mStandardEffect.Render(character03);
			break;
		}

		mStandardEffect.End();
	}
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::CollapsingHeader("Model Selection", ImGuiTreeNodeFlags_DefaultOpen))
	{
		int currentModelIndex = static_cast<int>(gCurrentModel);
		if (ImGui::Combo("Current Model", &currentModelIndex, gObjectNames, std::size(gObjectNames)))
		{
			gCurrentModel = static_cast<CurrentModel>(currentModelIndex);
		}

		ImGui::Checkbox("Draw Skeleton", &mDrawSkeleton);
	}

	ImGui::Separator();

	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat3("Direction#Light", &mDirectionalLight.direction.x, 0.01f))
		{
			mDirectionalLight.direction = Math::Normalize(mDirectionalLight.direction);
		}

		ImGui::ColorEdit4("Ambient#Light", &mDirectionalLight.ambient.r);
		ImGui::ColorEdit4("Diffuse#Light", &mDirectionalLight.diffuse.r);
		ImGui::ColorEdit4("Specular#Light", &mDirectionalLight.specular.r);
	}

	ImGui::Separator();

	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		std::vector<RenderObject>* currentRenderObjects = nullptr;

		switch (gCurrentModel)
		{
		case CurrentModel::Character01:
			currentRenderObjects = &character01.renderObjects;
			break;
		case CurrentModel::Character02:
			currentRenderObjects = &character02.renderObjects;
			break;
		case CurrentModel::Character03:
			currentRenderObjects = &character03.renderObjects;
			break;
		}

		if (currentRenderObjects)
		{
			for (uint32_t i = 0; i < currentRenderObjects->size(); ++i)
			{
				Material& material = (*currentRenderObjects)[i].material;
				std::string renderObjectId = "RenderObject " + std::to_string(i);
				ImGui::PushID(renderObjectId.c_str());
				if (ImGui::CollapsingHeader(renderObjectId.c_str()))
				{
					ImGui::LabelText("label", "Material:");
					ImGui::ColorEdit4("Emissive#Material", &material.emissive.r);
					ImGui::ColorEdit4("Ambient#Material", &material.ambient.r);
					ImGui::ColorEdit4("Diffuse#Material", &material.diffuse.r);
					ImGui::ColorEdit4("Specular#Material", &material.specular.r);
					ImGui::DragFloat("Shininess#Material", &material.shininess, 0.1f, 0.1f, 10000.0f);
				}
				ImGui::PopID();
			}
		}
	}

	ImGui::Separator();

	mStandardEffect.DebugUI();
	ImGui::End();
}

void GameState::UpdateCamera(float deltaTime)
{
	InputSystem* input = InputSystem::Get();
	const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 4.0f;
	const float turnSpeed = 0.5f;

	if (input->IsKeyDown(KeyCode::W)) { mCamera.Walk(moveSpeed * deltaTime); }
	else if (input->IsKeyDown(KeyCode::S)) { mCamera.Walk(-moveSpeed * deltaTime); }
	else if (input->IsKeyDown(KeyCode::D)) { mCamera.Strafe(moveSpeed * deltaTime); }
	else if (input->IsKeyDown(KeyCode::A)) { mCamera.Strafe(-moveSpeed * deltaTime); }
	else if (input->IsKeyDown(KeyCode::E)) { mCamera.Rise(moveSpeed * deltaTime); }
	else if (input->IsKeyDown(KeyCode::Q)) { mCamera.Rise(-moveSpeed * deltaTime); }

	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
	}
}
