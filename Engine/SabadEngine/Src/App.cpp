#include "Precompiled.h"
#include "App.h"
#include "AppState.h"

using namespace SabadEngine;
using namespace SabadEngine::Core;

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

	// Last Step Before Running
	ASSERT(mCurrentState != nullptr, "App: Need an app state to run");
	mCurrentState->Initialize();

	// Process Updates
	mRunning = true;
	while (mRunning)
	{
		myWindow.ProcessMessage();
		
		if (!myWindow.IsActive())
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