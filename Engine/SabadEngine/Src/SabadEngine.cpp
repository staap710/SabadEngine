#include "Precompiled.h"
#include "SabadEngine.h"

SabadEngine::App& SabadEngine::MainApp()
{
	static App sApp;
	return sApp;
}