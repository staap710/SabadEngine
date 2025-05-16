#pragma once

#include "MeshTypes.h"

namespace SabadEngine::Graphics
{

	class MeshBuilder
	{
	public:
		//cube
		static MeshPC CreateCubePC(float size, const Color& color);
		static MeshPC CreateCubePC(float size);

		//pyramid
		static MeshPC CreatePyramidPC(float size);

		//rectangle
		static MeshPC CreateRectanglePC(float width, float height, float depth);

		//plane
		static MeshPC CreatePlanePC(int numRows, int nyColums, float spacing, bool horizontal=true);
		
		// cylinder
		static MeshPC CreateCylinderPC(float radius, float height, int slices, int stacks);
		// sphere
		static MeshPC CreateSpherePC(float radius, int slices, int stacks);

	};
}