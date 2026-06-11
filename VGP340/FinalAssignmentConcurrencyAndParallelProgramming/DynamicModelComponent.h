#pragma once

#include <SabadEngine/Inc/SabadEngine.h>
#include "CustomTypeIds.h"
#include <atomic>
#include <memory>

class DynamicModelComponent : public SabadEngine::RenderObjectComponent
{
public:
    SET_TYPE_ID(CustomComponentId::DynamicModel);

    void Initialize() override;
    void Terminate() override;
    void Deserialize(const rapidjson::Value& value) override;

    SabadEngine::Graphics::ModelId GetModelId() const override;
    const SabadEngine::Graphics::Model& GetModel() const override;

    // Asynchronous loading/unloading controls (Main Thread)
    void SetDistanceThreshold(float dist) { mDistanceThreshold = dist; }
    float GetDistanceThreshold() const { return mDistanceThreshold; }

    bool IsLoaded() const { return mIsLoaded.load(); }
    bool IsLoading() const { return mIsLoading.load(); }
    bool IsRegistered() const { return mIsRegistered; }

    void AsyncLoadCPU(const std::filesystem::path& rootDir);
    void RegisterWithRenderService();
    void UnregisterFromRenderService();
    void UnloadCPU();

    const std::string& GetFileName() const { return mFileName; }

private:
    std::string mFileName;
    float mDistanceThreshold = 100.0f;

    bool mIsRegistered = false;
    std::atomic<bool> mIsLoading{ false };
    std::atomic<bool> mIsLoaded{ false };

    std::unique_ptr<SabadEngine::Graphics::Model> mCPUModel;
    SabadEngine::Graphics::ModelId mModelId = 0;

    std::vector<std::string> mAnimations;
};
