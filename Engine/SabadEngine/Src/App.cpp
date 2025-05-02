#include "Precompiled.h"
#include "App.h"
#include "AppState.h"

using namespace SabadEngine;
using namespace SabadEngine::Core;
using namespace SabadEngine::Graphics;
using namespace SabadEngine::Input;

void App::Run(const AppConfig& config)
{
	LOG("App Started");

	// Initialize Everything
	Window myWindow;
	myWindow.Initialize(
		GetModuleHandle(nullptr),
		config.appName,
		config.winWidth,
		config.winHeight
		);
	auto handle = myWindow.GetWindowHandle();
	GraphicsSystem::StaticInitialize(handle, false);
	InputSystem::StaticInitialize(handle);
	// Last Step Before Running
	ASSERT(mCurrentState != nullptr, "App: Need an app state to run");
	mCurrentState->Initialize();

	// Process Updates
	InputSystem* input = InputSystem::Get();
	mRunning = true;
	while (mRunning)
	{
		myWindow.ProcessMessage();
		input->Update();
		if (!myWindow.IsActive()||input-> IsKeyPressed(KeyCode::ESCAPE))
		{
			Quit();
			continue;
		}
	}

	if (mNextState != nullptr)
	{
		mCurrentState->Terminate();
		mCurrentState = std::exchange(mNextState, nullptr);
		mCurrentState->Initialize();
	}

	float deltaTime = TimeUtil::GetDeltaTime();
#if defined(_DEBUG)
	if (deltaTime < 0.5f) 
#endif
	{
		mCurrentState->Update(deltaTime);
	}
	GraphicsSystem* gs = GraphicsSystem::Get();
	gs->BeginRender();
	mCurrentState->Render();
	gs->EndRender();


	// Terminate Everything
	LOG("App Quit");
	
	mCurrentState->Terminate();
	
	myWindow.Terminate();
}

void App::Quit()
{
	mRunning = false;
}

void App::ChangeState(const std::string& stateName)
{
	auto iter = mAppStates.find(stateName);
	if (iter != mAppStates.end())
	{
		mNextState = iter->second.get();
	}
}