#pragma once

#include "MeshBuffer.h"
#include "Transform.h"
#include "Material.h"
#include "TextureManager.h"
#include "ModelManager.h"

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

    class RenderGroup
    {
    public:
        void Initialize(const std::filesystem::path& modelFilePath);
        void Terminate();

        ModelId modelId;
		Transform transform;
		std::vector<RenderObject> renderObjects;
    };
}