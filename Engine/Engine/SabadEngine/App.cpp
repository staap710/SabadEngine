#include "App.h"

void App::Run()
{
	LOG("App started");
	mRunning = true;
	while (mRunning)
	{
		LOG("App running");
	}
	LOG("App stopped");
}

void App::Quit()
{
}
