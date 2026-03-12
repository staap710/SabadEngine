#pragma once

#include <SabadEngine/Inc/SabadEngine.h>

enum class GameEventType
{
	CollisionEvent = 1,
	FireworkEvent
};

class CollisionEvent : public SabadEngine::Core::Event
{
public:
	CollisionEvent() {}
	SET_EVENT_TYPE_ID(GameEventType::CollisionEvent)
};

class FireworkEvent : public SabadEngine::Core::Event
{
public:
	FireworkEvent() {}
	SET_EVENT_TYPE_ID(GameEventType::FireworkEvent)
};