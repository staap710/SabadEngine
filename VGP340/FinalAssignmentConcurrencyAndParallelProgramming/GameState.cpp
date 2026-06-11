#include "GameState.h"
#include <SabadEngine/Inc/TransformComponent.h>
#include <SabadEngine/Inc/CameraService.h>
#include <SabadEngine/Inc/GameObjectFactory.h>
#include <string>

using namespace SabadEngine;
using namespace SabadEngine::Graphics;
using namespace SabadEngine::Input;
using namespace SabadEngine::Physics;

namespace
{
    Component* MakeCustomComponent(const std::string& componentName, GameObject& gameObject)
    {
        if (componentName == "DynamicModelComponent")
        {
            return gameObject.AddComponent<DynamicModelComponent>();
        }
        return nullptr;
    }

    Component* GetCustomComponent(const std::string& componentName, GameObject& gameObject)
    {
        if (componentName == "DynamicModelComponent")
        {
            return gameObject.GetComponent<DynamicModelComponent>();
        }
        return nullptr;
    }
}

void GameState::Initialize()
{
    // Use a custom level file
    mLevelFile = L"../../Assets/Templates/Levels/concurrency_level.json";

    // Set custom callbacks to instantiate and access our custom DynamicModelComponent
    GameObjectFactory::SetCustomMake(MakeCustomComponent);
    GameObjectFactory::SetCustomGet(GetCustomComponent);

    // Initialize the GameWorld
    mGameWorld.LoadLevel(mLevelFile);

    // Initialize Thread Pool with CPU hardware threads
    unsigned int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0)
    {
        numThreads = 4;
    }
    mThreadPool = std::make_unique<ThreadPool>(numThreads);

    // cap concurrent background loads relative to threads
    mMaxConcurrentBackgroundLoads = static_cast<int>(std::max<unsigned int>(1u, numThreads / 2));

    // Spawn a large grid of dynamic model objects (30x30 = 900 objects)
    const float spacing = 20.0f;
    const int gridSize = 30;
    mTelemetryTotalObjects = gridSize * gridSize;

    for (int x = 0; x < gridSize; ++x)
    {
        for (int z = 0; z < gridSize; ++z)
        {
            float posX = (x - gridSize / 2.0f) * spacing;
            float posZ = (z - gridSize / 2.0f) * spacing;

            std::string name = "GridObj_" + std::to_string(x) + "_" + std::to_string(z);
            GameObject* go = mGameWorld.CreateGameObject(name, "../../Assets/Templates/Objects/dynamic_model_obj.json");
            if (go != nullptr)
            {
                TransformComponent* tc = go->GetComponent<TransformComponent>();
                if (tc != nullptr)
                {
                    tc->position = { posX, 0.0f, posZ };
                }

                // Call components initialize
                go->Initialize();

                DynamicModelComponent* dmc = go->GetComponent<DynamicModelComponent>();
                if (dmc != nullptr)
                {
                    mDynamicModels.push_back(dmc);
                }
            }
        }
    }
}

void GameState::Terminate()
{
    mThreadPool.reset();
    mDynamicModels.clear();
    mGameWorld.Terminate();
}

void GameState::Update(float deltaTime)
{
    // Update camera and basic engine logic
    mGameWorld.Update(deltaTime);

    // Get the Camera position
    CameraService* cameraService = mGameWorld.GetService<CameraService>();
    if (cameraService == nullptr)
    {
        return;
    }

    Math::Vector3 cameraPos = cameraService->GetMain().GetPosition();

    // ---------------------------------------------------------
    // CONCURRENCY & PARALLEL PROGRAMMING PIPELINE
    // ---------------------------------------------------------
    // Phase 1: Parallel distance calculation & CPU loading triggers
    // We split our 900 dynamic models into chunks and execute them on multiple threads.
    size_t totalModels = mDynamicModels.size();
    size_t numWorkers = mThreadPool->GetTotalThreadCount();
    size_t chunkSize = (totalModels + numWorkers - 1) / numWorkers;

    std::vector<std::future<void>> futures;
    mTelemetryLoadingCount = 0;

    for (size_t w = 0; w < numWorkers; ++w)
    {
        size_t startIdx = w * chunkSize;
        size_t endIdx = std::min(startIdx + chunkSize, totalModels);
        if (startIdx >= totalModels)
        {
            break;
        }

        // Enqueue a chunk distance check task
        futures.push_back(mThreadPool->Enqueue([this, startIdx, endIdx, cameraPos]() {
            for (size_t i = startIdx; i < endIdx; ++i)
            {
                DynamicModelComponent* dmc = mDynamicModels[i];
                TransformComponent* tc = dmc->GetOwner().GetComponent<TransformComponent>();
                if (tc == nullptr)
                {
                    continue;
                }

                // Perform parallel distance calculation
                float dist = Math::Distance(cameraPos, tc->position);

                // Update thresholds dynamically
                dmc->SetDistanceThreshold(mGlobalDistanceThreshold);

                if (dist < mGlobalDistanceThreshold)
                {
                    if (!dmc->IsLoaded() && !dmc->IsLoading())
                    {
                        // Throttle background loads: only enqueue if we can reserve a slot.
                        int prev = mConcurrentBackgroundLoads.fetch_add(1);
                        if (prev < mMaxConcurrentBackgroundLoads)
                        {
                            // We reserved a slot — schedule a background load that will release the slot when done.
                            mThreadPool->Enqueue([this, dmc]() {
                                dmc->AsyncLoadCPU("../../Assets/Models");
                                mConcurrentBackgroundLoads.fetch_sub(1);
                            });
                        }
                        else
                        {
                            // Too many background loads right now — undo reservation and skip this frame.
                            mConcurrentBackgroundLoads.fetch_sub(1);
                        }
                    }
                }

                if (dmc->IsLoading())
                {
                    mTelemetryLoadingCount++;
                }
            }
        }));
    }

    // Wait for the parallel distance checks to complete
    for (auto& fut : futures)
    {
        fut.wait();
    }

    // Phase 2: Main thread updates (Registration with RenderService & GPU resource creation)
    mTelemetryLoadedCount = 0;
    mTelemetryRenderedCount = 0;

    for (DynamicModelComponent* dmc : mDynamicModels)
    {
        TransformComponent* tc = dmc->GetOwner().GetComponent<TransformComponent>();
        if (tc == nullptr)
        {
            continue;
        }

        float dist = Math::Distance(cameraPos, tc->position);

        if (dist < mGlobalDistanceThreshold)
        {
            if (dmc->IsLoaded())
            {
                // Register and create mesh buffer/textures safely on main thread
                dmc->RegisterWithRenderService();
                mTelemetryLoadedCount++;
                mTelemetryRenderedCount++;
            }
        }
        else
        {
            // Unregister and unload model to free memory
            if (dmc->IsRegistered())
            {
                dmc->UnregisterFromRenderService();
            }
            if (dmc->IsLoaded())
            {
                dmc->UnloadCPU();
            }
        }
    }
}

void GameState::Render()
{
    mGameWorld.Render();
}

void GameState::DebugUI()
{
    ImGui::Begin("Concurrency Dashboard", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Text("=== TELEMETRY ===");
    ImGui::Text("Total Grid Spawns: %d", mTelemetryTotalObjects);
    ImGui::Text("Active Background CPU Loads: %d", mTelemetryLoadingCount.load());
    ImGui::Text("Models Loaded in Memory: %d", mTelemetryLoadedCount);
    ImGui::Text("Models Currently Rendered: %d", mTelemetryRenderedCount);

    ImGui::Separator();
    ImGui::Text("=== THREAD POOL CONFIG ===");
    ImGui::Text("Worker Threads Count: %zu", mThreadPool->GetTotalThreadCount());
    ImGui::Text("Active Worker Threads: %zu", mThreadPool->GetActiveThreadCount());
    ImGui::Text("Queue Size (Pending Tasks): %zu", mThreadPool->GetQueueSize());

    ImGui::Separator();
    ImGui::Text("=== SETTINGS ===");
    ImGui::SliderFloat("Load Distance X", &mGlobalDistanceThreshold, 10.0f, 400.0f, "%.1f");
    ImGui::SliderInt("Max Background Loads", &mMaxConcurrentBackgroundLoads, 1, 32);

    ImGui::End();

    // Standard GameWorld services debug UI
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    mGameWorld.DebugUI();
    ImGui::End();
}
