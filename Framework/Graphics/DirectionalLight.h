#pragma once
#include "Color.h"

namespace SabadEngine::Graphics
{
	struct DirectionalLight
	{
		Color ambient = Colors::White; //any light color without light direction
		Color diffuse = Colors::White;	//main light color with light direction
		Color specular = Colors::White; //reflection light color
		Math::Vector3 direction = { 1.0f, -1.0f, 1.0f };
		float padding;
	};
}
{

}