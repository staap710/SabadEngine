#pragma once
#include "Color.h"
namespace SabadEngine::Graphics
{
	struct Material
	{
	public:
		Color emissive = Colors::Black; // Light emitted from the surface
		Color ambient = Colors::White;   // Any light color without light contact
		Color diffuse = Colors::White;   // Base light color
		Color specular = Colors::White;  // Highlight light color
		float shininess = 10.0f;             // Sharpness of the specular highlight
		float padding[3] = { 0.0f, 0.0f, 0.0f }; // Padding to maintain the 16 byte alignment
	};
}