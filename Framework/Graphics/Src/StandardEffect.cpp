#include "Precompiled.h"
#include "StandardEffect.h"

#include "VertexTypes.h"
#include "Camera.h"
#include "RenderObject.h"

using namespace SabadEngine;
using namespace SabadEngine::Graphics;

void StandardEffect::Initialize(const std::filesystem::path& path)
{
	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();

	mVertexShader.Initialize<Vertex>(path);
	mPixelShader.Initialize(path);
	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Wrap);
}

void StandardEffect::Terminate()
{	
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();
}	

void StandardEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mSampler.BindVS(0);
	mSampler.BindPS(0);


	mTransformBuffer.BindVS(0);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);
	mMaterialBuffer.BindPS(2);
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
	data.world = Math::Transpose(matWorld);
	data.viewPosition = mCamera->GetPosition();
	mTransformBuffer.Update(data);

	mLightBuffer.Update(*mDirectionalLight);

	renderObject.meshBuffer.Render();
}

void StandardEffect::SetCamera(const Camera& camera)
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