#include <SabadEngine/Inc/SabadEngine.h>
#include "ShapeState.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    SabadEngine::AppConfig config;
    config.appName = L"Hello Shapes";

    SabadEngine::App& myApp = SabadEngine::MainApp();
    myApp.AddState<ShapeState>("ShapeState");
    myApp.Run(config);

    return 0;
}