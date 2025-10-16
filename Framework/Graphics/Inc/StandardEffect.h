#pragma once

#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "Sampler.h"

namespace SabadEngine::Graphics
{
    class Camera;
    class RenderObject;

    class StandardEffect final
    {
    public:
        void Initialize(const std::filesystem::path& path);
        void Terminate();

        void Begin();
        void End();

        void Render(const RenderObject& renderObject);

        void SetCamera(const Camera& camera);

        void SetDirectionalLight(const DirectionalLight& directionalLight);

        void DebugUI();

    private:

        struct TransformData
        {
            Math::Matrix4 wvp; // World-View-Projection matrix
            Math::Matrix4 world; // World matrix
            Math::Vector3 viewPosition; // Camera position in world space
            float padding = 0.0f; // Padding to maintain the 16 byte alignment
        };

        using TransformBuffer = TypedConstantBuffer<TransformData>;
        TransformBuffer mTransformBuffer;

        using LightBuffer = TypedConstantBuffer<DirectionalLight>;
        LightBuffer mLightBuffer;

		using MaterialBuffer = TypedConstantBuffer<Material>;
		MaterialBuffer mMaterialBuffer;

        VertexShader mVertexShader;
        PixelShader mPixelShader;
		Sampler mSampler;

        const Camera* mCamera = nullptr;
        const DirectionalLight* mDirectionalLight = nullptr;
    };
}