#include <SabadEngine/Inc/SabadEngine.h>
#include "ShapeState.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    SabadEngine::AppConfig config;
    config.appName = L"Hello Shapes";

    config.winWidth = 1200;
    config.winHeight = 720;

    SabadEngine::App& myApp = SabadEngine::MainApp();

    myApp.AddState<TriForce>("TriForce");
    myApp.AddState<House>("House");
    myApp.AddState<Heart>("Heart");

    myApp.Run(config);

    return 0;
}