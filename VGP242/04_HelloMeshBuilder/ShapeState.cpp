#include "ShapeState.h"


using namespace SabadEngine;
using namespace SabadEngine::Graphics;
using namespace SabadEngine::Math;
using namespace SabadEngine::Input;


void ShapeState::Initialize()
{

	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mTransformBuffer.Initialize(sizeof(Matrix4));

	//creates a shape out of the vertices
	CreateShape();
	mMeshBuffer.Initialize(mMesh);


	//BIND TO FUNCTION IN SPECIFIED SHADER FILE
	std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoTexture.fx";
	mVertexShader.Initialize<VertexPX>(shaderFilePath);
	mPixelShader.Initialize(shaderFilePath);

	mTexture.Initialize(L"../../Assets/Textures/space.jpg");
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void ShapeState::Terminate()
{
	mSampler.Terminate();
	mTexture.Terminate();
	mMesh.vertices.clear();
	mTransformBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
}

void ShapeState::Update(float deltaTime)
{
	Input::InputSystem* input = Input::InputSystem::Get();
	const float moveSpeed = input->IsKeyDown(Input::KeyCode::LSHIFT) ? 10.0f : 1.0f;
	const float turnSpeed = 1.0f;

	if (input->IsKeyDown(Input::KeyCode::W)) mCamera.Walk(moveSpeed * deltaTime);
	else if (input->IsKeyDown(Input::KeyCode::S)) mCamera.Walk(-moveSpeed * deltaTime);

	if (input->IsKeyDown(Input::KeyCode::A)) mCamera.Strafe(-moveSpeed * deltaTime);
	else if (input->IsKeyDown(Input::KeyCode::D)) mCamera.Strafe(moveSpeed * deltaTime);

	if (input->IsKeyDown(Input::KeyCode::Q)) mCamera.Rise(-moveSpeed * deltaTime);
	else if (input->IsKeyDown(Input::KeyCode::E)) mCamera.Rise(moveSpeed * deltaTime);

	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
	}
}

void ShapeState::Render()
{
	//prepare GPU
	mVertexShader.Bind();
	mPixelShader.Bind();

	mSampler.BindPS(0);
	mTexture.BindPS(0);

	//sync transform buffer
	mTransformBuffer.BindVs(0);

	//update the buffer data
	Math::Matrix4 matWorld = Math::Matrix4::Identity;
	Math::Matrix4 matView = mCamera.GetViewMatrix();
	Math::Matrix4 matProjection = mCamera.GetProjectionMatrix();
	Math::Matrix4 matFinal = matWorld * matView * matProjection; //world view projection
	Math::Matrix4 wvp = Math::Transpose(matFinal);
	mTransformBuffer.Update(&wvp);

	//draw
	mMeshBuffer.Render();
}

void ShapeState::CreateShape() {

	//cube
	//mMesh = MeshBuilder::CreateCubePC(1.0f);
	//pyramid
	//mMesh = MeshBuilder::CreatePyramidPC(1.0f);
	//rectangle
	//mMesh = MeshBuilder::CreateRectanglePC(2.0f, 1.0f, 3.0f);
	//plane
	//mMesh = MeshBuilder::CreatePlanePC(10, 10, 1.0f);
	//cylinder
	//mMesh = MeshBuilder::CreateCylinderPC(5, 5);
	//sphere
	//mMesh = MeshBuilder::CreateSpherePC(30, 30, 1.0f);

	// sphere with texture
	//mMesh = MeshBuilder::CreateSpherePX(30, 30, 1.0f);
	// plane with texture
	//mMesh = MeshBuilder::CreatePlanePX(10, 10, 1.0f, true);
	//skybox
	mMesh = MeshBuilder::CreateSkyBoxSpherePX(30, 30, 200.0f);
}
