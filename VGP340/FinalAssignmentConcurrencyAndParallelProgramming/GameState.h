#pragma once

#include <SabadEngine/Inc/SabadEngine.h>
#include "ThreadPool.h"
#include "DynamicModelComponent.h"
#include <vector>

class GameState : public SabadEngine::AppState
{
public:
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Render() override;
    void DebugUI() override;

private:
    std::filesystem::path mLevelFile;
    SabadEngine::GameWorld mGameWorld;

    // Concurrency System
    std::unique_ptr<ThreadPool> mThreadPool;
    std::vector<DynamicModelComponent*> mDynamicModels;

    // Dynamic rendering threshold control
    float mGlobalDistanceThreshold = 100.0f;

    // Real-time telemetry for display
    int mTelemetryTotalObjects = 0;
    std::atomic<int> mTelemetryLoadingCount{ 0 };
    int mTelemetryLoadedCount = 0;
    int mTelemetryRenderedCount = 0;

    // Background-load throttling
    std::atomic<int> mConcurrentBackgroundLoads{ 0 };
    int mMaxConcurrentBackgroundLoads = 4;
};
