#pragma once


#include "Component.h"

namespace SabadEngine
{
	class UIComponent : public Component
	{
	public:
		virtual void Render() = 0;
	};
}