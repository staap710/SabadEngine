#include "DynamicModelComponent.h"
#include <SabadEngine/Inc/GameWorld.h>
#include <SabadEngine/Inc/GameObject.h>
#include <SabadEngine/Inc/RenderService.h>
#include <SabadEngine/Inc/SaveUtil.h>

using namespace SabadEngine;
using namespace SabadEngine::Graphics;

void DynamicModelComponent::Initialize()
{
    // Do NOT call RenderObjectComponent::Initialize() because it automatically 
    // registers us with the RenderService immediately, which would assert
    // because the model is not loaded yet on the CPU/GPU.
    mIsRegistered = false;
    mIsLoaded = false;
    mIsLoading = false;
}

void DynamicModelComponent::Terminate()
{
    UnregisterFromRenderService();
    UnloadCPU();
}

void DynamicModelComponent::Deserialize(const rapidjson::Value& value)
{
    RenderObjectComponent::Deserialize(value);

    mAnimations.clear();
    SaveUtil::ReadString("FileName", mFileName, value);
    SaveUtil::ReadStringArray("Animations", mAnimations, value);

    if (value.HasMember("DistanceThreshold"))
    {
        mDistanceThreshold = value["DistanceThreshold"].GetFloat();
    }
}

ModelId DynamicModelComponent::GetModelId() const
{
    return mModelId;
}

const Model& DynamicModelComponent::GetModel() const
{
    ASSERT(mCPUModel != nullptr, "DynamicModelComponent: Model is not loaded!");
    return *mCPUModel;
}

void DynamicModelComponent::AsyncLoadCPU(const std::filesystem::path& rootDir)
{
    if (mIsLoaded || mIsLoading)
    {
        return;
    }

    mIsLoading = true;

    // Create the CPU model
    mCPUModel = std::make_unique<Model>();
    std::filesystem::path fullPath = rootDir / mFileName;

    // Load the model, material, and skeleton from disk
    ModelIO::LoadModel(fullPath, *mCPUModel);
    ModelIO::LoadMaterial(fullPath, *mCPUModel);
    ModelIO::LoadSkeleton(fullPath, *mCPUModel);

    // Load all associated animations
    for (const std::string& animation : mAnimations)
    {
        std::filesystem::path animPath = rootDir / animation;
        ModelIO::LoadAnimation(animPath, *mCPUModel);
    }

    // Generate model ID
    mModelId = std::filesystem::hash_value(rootDir / mFileName);

    mIsLoaded = true;
    mIsLoading = false;
}

void DynamicModelComponent::RegisterWithRenderService()
{
    if (mIsRegistered || !mIsLoaded)
    {
        return;
    }

    RenderService* renderService = GetOwner().GetWorld().GetService<RenderService>();
    if (renderService != nullptr)
    {
        renderService->Register(this);
        mIsRegistered = true;
    }
}

void DynamicModelComponent::UnregisterFromRenderService()
{
    if (!mIsRegistered)
    {
        return;
    }

    RenderService* renderService = GetOwner().GetWorld().GetService<RenderService>();
    if (renderService != nullptr)
    {
        renderService->Unregister(this);
        mIsRegistered = false;
    }
}

void DynamicModelComponent::UnloadCPU()
{
    // Make sure we unregister from GPU rendering first
    UnregisterFromRenderService();

    if (mCPUModel != nullptr)
    {
        mCPUModel.reset();
    }
    mIsLoaded = false;
    mIsLoading = false;
}
