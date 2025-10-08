#pragma once

#include "MeshBuffer.h"
#include "Transform.h"
namespace SabadEngine::Graphics
{
	class RenderObject
	{
	public:
		void Terminate();
		Transform transform;
		MeshBuffer meshBuffer;
	};
}