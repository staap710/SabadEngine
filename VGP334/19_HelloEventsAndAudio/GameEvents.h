#pragma once

#include <SabadEngine/Inc/SabadEngine.h>

enum class GameEventType
{
	PressSpace = 1,
	PressEnter
};

class PressSpaceEvent : public SabadEngine::Core::Event
{
public:
	PressSpaceEvent() {}
	SET_EVENT_TYPE_ID(GameEventType::PressSpace)
};

class PressEnterEvent : public SabadEngine::Core::Event
{
public:
	PressEnterEvent() {}
	SET_EVENT_TYPE_ID(GameEventType::PressEnter)
};