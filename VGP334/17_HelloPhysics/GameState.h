#pragma once

#include <SabadEngine/Inc/SabadEngine.h>

class GameState : public SabadEngine::AppState
{
public:
    void Initialize() override;

    void Terminate() override;

    void Update(float deltaTime) override;

    void Render() override;

    void DebugUI() override;

private:

    void UpdateCamera(float deltaTime);

    SabadEngine::Graphics::Camera mCamera;
    SabadEngine::Graphics::DirectionalLight mDirectionalLight;

    // Ball info
    SabadEngine::Graphics::RenderObject mEarth;
    SabadEngine::Physics::CollisionShape mBallShape;
    SabadEngine::Physics::RigidBody mBallRigidBody;

    // Ground info
    SabadEngine::Graphics::RenderObject mGroundObject;
    SabadEngine::Physics::CollisionShape mGroundShape;
    SabadEngine::Physics::RigidBody mGroundRigidBody;

    struct BoxData
    {
        SabadEngine::Graphics::RenderObject box;
        SabadEngine::Physics::CollisionShape shape;
        SabadEngine::Physics::RigidBody rigidBody;
    };
    using Boxes = std::vector<BoxData>;
    Boxes mBoxes;

    SabadEngine::Graphics::StandardEffect mStandardEffect;

    // Cloth info
    SabadEngine::Graphics::RenderObject mCloth;
    SabadEngine::Graphics::Mesh mClothMesh;
    SabadEngine::Physics::SoftBody mClothSoftBody;

    // Cloth Ball info
    SabadEngine::Graphics::RenderObject mClothBall;
    SabadEngine::Graphics::Mesh mClothBallMesh;
    SabadEngine::Physics::SoftBody mClothBallSoftBody;
};