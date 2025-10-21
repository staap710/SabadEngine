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

        Transform transform;   // Location/ Orientation
        MeshBuffer meshBuffer; // Shape

        Material material;    // Light data

        TextureId diffuseMapId;  // Diffuse texture for an object
        TextureId specMapId;   // Specular map for an object
        TextureId normalMapId;   // Normal texture for an object
        TextureId bumpMapId;   // Height texture for an object
    };
}