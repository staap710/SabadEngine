#include <SabadEngine/Inc/SabadEngine.h>

class MainState : public SabadEngine::AppState
{
public:
    void Initialize()
    {
        LOG("MAIN STATE INITIALIZED");
        mLifeTime = 2.0f;
    }
    void Terminate() override
    {
        LOG("MAIN STATE TERMINATED");
    }
    void Update(float deltaTime) override
    {
        mLifeTime -= std::max(deltaTime, 0.01f);
        if (mLifeTime <= 0.0f)
        {
            SabadEngine::MainApp().ChangeState("GameState");
        }
    }
private:
    float mLifeTime = 0.0f;
};

class GameState : public SabadEngine::AppState
{
public:
    void Initialize()
    {
        LOG("MAIN STATE INITIALIZED");
        mLifeTime = 2.0f;
    }
    void Terminate() override
    {
        LOG("MAIN STATE TERMINATED");
    }
    void Update(float deltaTime) override
    {
        mLifeTime -= std::max(deltaTime, 0.01f);
        if (mLifeTime <= 0.0f)
        {
            SabadEngine::MainApp().ChangeState("MainState");
        }
    }
private:
    float mLifeTime = 0.0f;
};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    SabadEngine::AppConfig config;
    config.appName = L"Hello Window";

    SabadEngine::App& myApp = SabadEngine::MainApp();
    myApp.AddState<MainState>("MainState");
    myApp.AddState<GameState>("GameState");
    myApp.Run(config);

    return 0;
}