#pragma once

#pragma once

namespace SabadEngine::Core
{
	using EventTypeId = std::size_t;

	class Event
	{
	public:
		Event() = default;
		virtual ~Event() = default;

		virtual EventTypeId GetTypeId() const = 0;

	};
}

#define SET_EVENT_TYPE_ID(id)\
	static SabadEngine::Core::EventTypeId StaticGetTypeId() { return static_cast<SabadEngine::Core::EventTypeId>(id); }\
	SabadEngine::Core::EventTypeId GetTypeId() const override { return StaticGetTypeId(); }