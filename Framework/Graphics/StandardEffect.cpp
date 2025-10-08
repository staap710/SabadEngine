#include "Precompiled.h"
#include "StandardEffect.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace SabadEngine;
using namespace SabadEngine::Graphics;

void StandardEffect::Initialize(const std::filesystem::path& path)
{
	mTransformBuffer.Initialize();
	mVertexShader.Initialize<vertex>(path);
	mPixelShader.Initialize(path);
}

void StandardEffect::Terminate()
{
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mTransformBuffer.Terminate();
}
void StandardEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mTransformBuffer.BindVS(0);
}
void StandardEffect::End()
{
}
void StandardEffect::Render(const RenderObject& renderObject)
{
	const Math::Matrix4 matWorld = renderObject.transform.GetMatrix4();
	const Math::Matrix4 matView = mCamera->GetViewMatrix();
	const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();
	const Math::Matrix4 matFinal = matWorld * matView * matProj;

	TransformData data;
	data.wvp = Math::Transpose(matFinal);
	mTransformBuffer.Update(data);
	renderObject.meshBuffer.Render();
}
void standardEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}
void StandardEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
{ 
	mDirectionalLight = &directionalLight;
	
}
void StandardEffect::DebugUI()
{
}