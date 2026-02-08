#pragma once

#include "MeshBuffer.h"
#include "Transform.h"
#include "Material.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Animator.h"

namespace SabadEngine::Graphics
{
    struct Skeleton;

    class RenderObject
    {
    public:
        void Terminate();

        Transform transform;   
        MeshBuffer meshBuffer; 

        Material material;    

        TextureId diffuseMapId;
        TextureId specMapId;   
        TextureId normalMapId; 
        TextureId bumpMapId;   
    };

    class RenderGroup
    {
    public:
        void Initialize(const std::filesystem::path& modelFilePath);
        void Terminate();

        ModelId modelId; // Model Identifier
        Transform transform; // Root Transform (Other objects may have other transforms)
        std::vector<RenderObject> renderObjects; // All objects to render

        const Skeleton* skeleton; // Skeleton for animation
        const Animator* animator = nullptr; // Animator for animation
    };
}