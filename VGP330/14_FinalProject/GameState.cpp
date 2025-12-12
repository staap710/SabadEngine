#include "GameState.h"

using namespace SabadEngine;
using namespace SabadEngine::Graphics;
using namespace SabadEngine::Input;

enum class CurrentObject
{
    Earth,
    Metal,
    Wood
};

const char* gObjectNames[] = { "Earth", "Metal", "Wood" };

CurrentObject gCurrentObject = CurrentObject::Earth;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });
    mDirectionalLight.ambient = { 0.4f, 0.4f, 0.4f, 1.0f };
    mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
    mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

    mCharacter.Initialize("Character01/Character01.model");

    Mesh groundMesh = MeshBuilder::CreatePlane(10, 10, 1.0f);
    mGround.meshBuffer.Initialize(groundMesh);
    mGround.diffuseMapId = TextureManager::Get()->LoadTexture("misc/concrete.jpg");

	MeshPX screenQuad = MeshBuilder::CreateScreenQuadPX();
	mScreenQuad.meshBuffer.Initialize(screenQuad);


    std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFile);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

    shaderFile = L"../../Assets/Shaders/FinalAssignmentShader.fx";
    mPostProcessingEffect.Initialize(shaderFile);

	mPostProcessingEffect.SetTexture(&mRenderTarget);
    


	//mPostProcessingEffect.SetTexture(&mCombineTexture, 1);

    
    GraphicsSystem* gs = GraphicsSystem::Get();
    const uint32_t screenWidth = gs->GetBackBufferWidth();
    const uint32_t screenHeight = gs->GetBackBufferHeight();
    mRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);
	//mCombineTexture.Initialize(L"../../Assets/Textures/BloodyScreenEffects/Effect_6.png");
}

void GameState::Terminate()
{
    mRenderTarget.Terminate();
	mScreenQuad.meshBuffer.Terminate();
    mCharacter.Terminate();
    mStandardEffect.Terminate();
    mGround.Terminate();
	mScreenQuad.Terminate();
}

void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
}

void GameState::Render()
{
    mRenderTarget.BeginRender();
    mStandardEffect.Begin();
    mStandardEffect.Render(mCharacter);
    mStandardEffect.Render(mGround);
    mStandardEffect.End();
    mRenderTarget.EndRender();

	mPostProcessingEffect.Begin();
	mPostProcessingEffect.Render(mScreenQuad);
	mPostProcessingEffect.End();

}

void GameState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
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

    if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
    {
        for (uint32_t i = 0; i < mCharacter.renderObjects.size(); ++i)
        {
            RenderObject& renderObject = mCharacter.renderObjects[i];
            Material& material = mCharacter.renderObjects[i].material;
            std::string renderObjectId = std::to_string(i);
            ImGui::PushID(renderObjectId.c_str());
            if (ImGui::CollapsingHeader(renderObjectId.c_str()))
            {
                ImGui::ColorEdit4("Emissive#Material", &material.emissive.r);
                ImGui::ColorEdit4("Ambient#Material", &material.ambient.r);
                ImGui::ColorEdit4("Diffuse#Material", &material.diffuse.r);
                ImGui::ColorEdit4("Specular#Material", &material.specular.r);
                ImGui::DragFloat("Shininess#Material", &material.shininess, 1.0f, 1.0f, 100.0f);
            }
            ImGui::PopID();
        }
    }

    ImGui::Separator();
    ImGui::Text("Render target");
    ImGui::Image(
        mRenderTarget.GetRawData(),
        { 128, 128 },
        { 0, 0 },
        { 1,1 },
        { 1, 1, 1, 1 },
        { 1, 1, 1, 1 });
    mStandardEffect.DebugUI();
    mPostProcessingEffect.DebugUI();
    ImGui::End();
}

void GameState::UpdateCamera(float deltaTime)
{
    // Camera Controls:
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