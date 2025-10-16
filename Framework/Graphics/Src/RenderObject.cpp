#include "Precompiled.h"
#include "RenderObject.h"

using namespace SabadEngine;
using namespace SabadEngine::Graphics;

void RenderObject::Terminate()
{
	meshBuffer.Terminate();
	TextureManager* tm = TextureManager::Get();
	tm->ReleaseTexture(diffuseMapId);

}