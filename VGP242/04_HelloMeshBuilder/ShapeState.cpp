#include "ShapeState.h"

using namespace SabadEngine;
using namespace SabadEngine::Math;
using namespace SabadEngine::Graphics;
using namespace SabadEngine::Input;


void ShapeState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mTransformBuffer.Initialize(sizeof(Math::Matrix4));

	// Creates a shape out of the vertices
	CreateShape();
	mMeshbuffer.Initialize(mMesh);

	std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoTexture.fx";
	mVertexShader.Initialize<VertexPX>(shaderFilePath);
	mPixelShader.Initialize(shaderFilePath);

	mTexture.Initialize(L"../../Assets/Textures/skysphere/space.jpg");
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void ShapeState::Terminate()
{
	mTransformBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshbuffer.Terminate();
	mSampler.Terminate();
	mTexture.Terminate();
}

void ShapeState::Update(float deltaTime)
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

	// Scene Change Controls:
	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::UP))
	{
		SabadEngine::MainApp().ChangeState("Pyramid");
	}

	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::LEFT))
	{
		SabadEngine::MainApp().ChangeState("Cube");
	}

	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::RIGHT))
	{
		SabadEngine::MainApp().ChangeState("Rectangle");
	}
}

void ShapeState::Render()
{
	// Prepare GPU
	mVertexShader.Bind();
	mPixelShader.Bind();

	mSampler.BindPS(0);
	mTexture.BindPS(0);

	// Sync Transform Buffer
	mTransformBuffer.BindVS(0);

	// Update the Buffer Data
	Math::Matrix4 matWorld = Math::Matrix4::Identity;
	Math::Matrix4 matView = mCamera.GetViewMatrix();
	Math::Matrix4 matProj = mCamera.GetProjectionMatrix();
	Math::Matrix4 matFinal = matWorld * matView * matProj; // = wvp
	Math::Matrix4 wvp = Math::Transpose(matFinal);
	mTransformBuffer.Update(&wvp);

	// Draw
	mMeshbuffer.Render();
}

void ShapeState::CreateShape()
{
	// mMesh = MeshBuilder::CreateRectanglePC(2.0f, 1.0f, 2.0f);
	// mMesh = MeshBuilder::CreateCubePC(4.0f);
	// mMesh = MeshBuilder::CreatePyramidPC(5.0f);
	// mMesh = MeshBuilder::CreatePlanePC(10.f, 10.f, 1);
	// mMesh = MeshBuilder::CreateCylinderPC(25.0f, 5.0f);
	// mMesh = MeshBuilder::CreateSpherePC(30, 30, 1.0f);

	mMesh = MeshBuilder::CreateSkySpherePX(30, 30, 250.0f);
	// mMesh = MeshBuilder::CreatePlanePX(30, 30, 1.0f);
	// mMesh = MeshBuilder::CreateSpherePX(30, 30, 1.0f);
}