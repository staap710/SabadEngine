#pragma once

#include "MeshBuffer.h"
#include "Transform.h"
#include "Material.h"
#include "TextureManager.h"

namespace SabadEngine::Graphics
{
    class RenderObject
    {
    public:
        void Terminate();

        Transform transform; 
        MeshBuffer meshBuffer; 
        Material material;
        TextureId diffuseMapId;
		TextureId specularMapId;
    };
}