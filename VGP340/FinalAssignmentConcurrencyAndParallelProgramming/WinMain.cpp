#include "GameState.h"

using namespace SabadEngine;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	AppConfig config;
	config.appName = L"Concurrent Parallel Model Loader";
	config.winWidth = 1280;
	config.winHeight = 720;
	config.maxVertexCount = 500000; // Increase vertex capacity since we render many models

	App& myApp = MainApp();
	myApp.AddState<GameState>("GameState");
	myApp.Run(config);

	return 0;
}
