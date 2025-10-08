#pragma once

#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

namespace SabadEngine::Graphics {
	class Camera;
	class RenderObject;
	class StandardEffect final {
	public:
		void Initialize(const std::filesystem::path& path);
		void Terminate();
		void Begin();
		void End();
		void Render(const RenderObject& renderObject);
		void SetCamera(const Camera& camera);
		void SetDirectionalLight(const directionalLight& directionalLight);
		void DebugUI();
	private:
		struct TransformData
		{
			Math::Matrix4 wvp;
		};
		using TransformBuffer = TypedConstantBuffer<TransformData>;
		TransformBuffer mTransformBuffer;
		using LightBuffer = TypedConstantBuffer <DirectionalLight>;
		LightBuffer mLightBuffer;

		VertexShader mVertexShader;
		PixelShader mPixelShader;
		const Camera* mCamera = nullptr;
		ConstantBuffer mTransformBuffer;
}