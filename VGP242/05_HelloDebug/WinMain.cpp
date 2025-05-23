#include "GameState.h"
using namespace SabadEngine;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int nCmdShow)
{
	// Initialize the App
	AppConfig config;
	config.appName = L "Hello Debug";
	
	App& myApp = MainApp();
	myApp.AddState<GameState>("GameState");

	myApp.Run(config);
	return 0;
}