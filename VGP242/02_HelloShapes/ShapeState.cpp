#include "ShapeState.h"

using namespace SabadEngine;
using namespace SabadEngine::Math;
using namespace SabadEngine::Graphics;

void ShapeState::Initialize()
{
	// Creates a shape out of the vertices
	CreateShape();

	auto device = GraphicsSystem::Get()->GetDevice();

	// Need to create a buffer to store the vertices
	// STORES DATA FOR THE OBJECT
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = static_cast<UINT>(mVertices.size()) * sizeof(Vertex);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = mVertices.data();

	HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &mVertexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create Vertex Buffer");
	//====================================================================================================

	// BIND TO FUNCTION IN SPECIFIED SHADER FILE
	std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoColor.fx";

	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	hr = D3DCompileFromFile(
		shaderFilePath.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VS", "vs_5_0",
		shaderFlags, 0,
		&shaderBlob,
		&errorBlob);

	if (errorBlob != nullptr && errorBlob->GetBufferPointer() != nullptr)
	{
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	}
	ASSERT(SUCCEEDED(hr), "Failed to create Vertex Shader");

	hr = device->CreateVertexShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mVertexShader);
	ASSERT(SUCCEEDED(hr), "Failed to create Vertex Shader");
	//======================================================================================================

	// STATE WHAT THE VERTEX VARIABLES ARE
	std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout;
	vertexLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT });
	vertexLayout.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT });

	hr = device->CreateInputLayout(
		vertexLayout.data(),
		static_cast<UINT>(vertexLayout.size()),
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		&mInputLayout);
	ASSERT(SUCCEEDED(hr), "Failed to create Input Layout");
	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
	//======================================================================================================

	// BIND TO PIXEL FUNCTION IN SPECIFIED SHADER FILE
	hr = D3DCompileFromFile(
		shaderFilePath.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS", "ps_5_0",
		shaderFlags, 0,
		&shaderBlob,
		&errorBlob);

	if (errorBlob != nullptr && errorBlob->GetBufferPointer() != nullptr)
	{
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	}
	ASSERT(SUCCEEDED(hr), "Failed to create Pixel Shader");

	hr = device->CreatePixelShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mPixelShader);
	ASSERT(SUCCEEDED(hr), "Failed to create Pixel Shader");
	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}

void ShapeState::Terminate()
{
	mVertices.clear();
	SafeRelease(mPixelShader);
	SafeRelease(mInputLayout);
	SafeRelease(mVertexShader);
	SafeRelease(mVertexBuffer);
}

void ShapeState::Render()
{
	auto context = GraphicsSystem::Get()->GetContext();
	// Bind buffers
	context->VSSetShader(mVertexShader, nullptr, 0);
	context->IASetInputLayout(mInputLayout);
	context->PSSetShader(mPixelShader, nullptr, 0);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Draw
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	context->Draw(static_cast<UINT>(mVertices.size()), 0);
}

void ShapeState::Update(float deltaTime)
{
	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::UP))
	{
		SabadEngine::MainApp().ChangeState("TriForce");
	}

	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::LEFT))
	{
		SabadEngine::MainApp().ChangeState("House");
	}

	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::RIGHT))
	{
		SabadEngine::MainApp().ChangeState("Heart");
	}
}

void ShapeState::CreateShape()
{
	mVertices.push_back({ { -0.5f,0.0f,0.0f }, Colors::Red });
	mVertices.push_back({ {  0.0f,0.75f,0.0f }, Colors::Yellow });
	mVertices.push_back({ {  0.5f,0.0f,0.0f }, Colors::Red });

	mVertices.push_back({ { -0.5f,0.0f,0.0f }, Colors::Red });
	mVertices.push_back({ {  0.5f,0.0f,0.0f }, Colors::Yellow });
	mVertices.push_back({ {  0.0f, -0.75f,0.0f }, Colors::Red });
}

void TriForce::Update(float deltaTime)
{
	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::DOWN))
	{
		SabadEngine::MainApp().ChangeState("ShapeState");
	}

	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::LEFT))
	{
		SabadEngine::MainApp().ChangeState("House");
	}

	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::RIGHT))
	{
		SabadEngine::MainApp().ChangeState("Heart");
	}
}

void TriForce::CreateShape()
{
	//Left ear
	mVertices.push_back({ { -0.4f, 0.8f, 0.0f }, Colors::Orange });
	mVertices.push_back({ { -0.3f, 0.6f, 0.0f }, Colors::Orange });
	mVertices.push_back({ { -0.4f, 0.5f, 0.0f }, Colors::Orange });
	//Right ear
	mVertices.push_back({ { -0.2f, 0.8f, 0.0f }, Colors::Orange });
	mVertices.push_back({ { -0.2f, 0.5f, 0.0f }, Colors::Orange });
	mVertices.push_back({ { -0.3f, 0.6f, 0.0f }, Colors::Orange });
	//face up
	mVertices.push_back({ { -0.3f, 0.6f, 0.0f }, Colors::Orange });
	mVertices.push_back({ { -0.2f, 0.5f, 0.0f }, Colors::Orange });
	mVertices.push_back({ { -0.4f, 0.5f, 0.0f }, Colors::Orange });
	//face down
	mVertices.push_back({ { -0.4f, 0.5f, 0.0f }, Colors::Orange });
	mVertices.push_back({ { -0.2f, 0.5f, 0.0f }, Colors::Orange });
	mVertices.push_back({ { -0.3f, 0.4f, 0.0f }, Colors::Orange });
	//Body
	mVertices.push_back({ { -0.3f, 0.4f, 0.0f }, Colors::Red});
	mVertices.push_back({ { -0.2f, 0.0f, 0.0f }, Colors::Red });
	mVertices.push_back({ { -0.4f, 0.0f, 0.0f }, Colors::Red });

}

void House::Update(float deltaTime)
{
	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::DOWN))
	{
		SabadEngine::MainApp().ChangeState("ShapeState");
	}

	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::UP))
	{
		SabadEngine::MainApp().ChangeState("TriForce");
	}

	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::RIGHT))
	{
		SabadEngine::MainApp().ChangeState("Heart");
	}
}

void House::CreateShape()
{
	//Triangle Up
	mVertices.push_back({ { -0.5f, 0.5f, 0.0f }, Colors::Violet });
	mVertices.push_back({ { 0.5f, 0.5f, 0.0f }, Colors::Violet });
	mVertices.push_back({ { 0.0f, -0.8f, 0.0f }, Colors::Yellow });

	//Triangle Down
	mVertices.push_back({ { 0.0f, 0.8f, 0.0f }, Colors::Yellow });
	mVertices.push_back({ { 0.5f, -0.5f, 0.0f }, Colors::GreenYellow });
	mVertices.push_back({ { -0.5f, -0.5f, 0.0f }, Colors::GreenYellow });

	//Background
	mVertices.push_back({ { -2.0f, 1.0f, 0.0f }, Colors::White });
	mVertices.push_back({ { 2.0f, 1.0f, 0.0f }, Colors::Silver });
	mVertices.push_back({ { 0.0f, -3.0f, 0.0f }, Colors::Gold });

}

void Heart::Update(float deltaTime)
{
	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::DOWN))
	{
		SabadEngine::MainApp().ChangeState("ShapeState");
	}

	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::LEFT))
	{
		SabadEngine::MainApp().ChangeState("House");
	}

	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::UP))
	{
		SabadEngine::MainApp().ChangeState("TriForce");
	}
}

void Heart::CreateShape()
{
	// Top left lobe
	mVertices.push_back({ { -0.4f, 0.1f, 0.0f }, Colors::PaleVioletRed });
	mVertices.push_back({ { -0.15f, 0.5f, 0.0f }, Colors::DarkRed });
	mVertices.push_back({ { 0.1f, 0.1f, 0.0f }, Colors::PaleVioletRed });

	// Top right lobe
	mVertices.push_back({ { 0.15f, 0.5f, 0.0f }, Colors::DarkRed });
	mVertices.push_back({ { 0.4f, 0.1f, 0.0f }, Colors::PaleVioletRed });
	mVertices.push_back({ { -0.1f, 0.1f, 0.0f }, Colors::PaleVioletRed });

	// Bottom triangle
	mVertices.push_back({ { -0.4f, 0.1f, 0.0f },Colors::PaleVioletRed });
	mVertices.push_back({ { 0.4f, 0.1f, 0.0f }, Colors::PaleVioletRed });
	mVertices.push_back({ { 0.0f, -0.65f, 0.0f },Colors::DarkRed });
}