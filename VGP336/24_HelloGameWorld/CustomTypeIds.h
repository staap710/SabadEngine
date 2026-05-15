#pragma once

#include <SabadEngine/Inc/TypeIds.h>

enum class CustomComponentId
{
	CustomDebugDraw = static_cast<int>(SabadEngine::ComponentId::Count)
};

enum class CustomServiceId
{
	CustomDebugDrawDisplay = static_cast<int>(SabadEngine::ServiceId::Count)
};