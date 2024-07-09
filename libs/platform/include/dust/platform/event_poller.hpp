#pragma once

#include <optional>

#include <dust/platform/event.hpp>

namespace dust::platform
{

class EventPoller
{
public:
	EventPoller() = default;
	EventPoller(const EventPoller &) = delete;
	virtual ~EventPoller() noexcept = 0;

	EventPoller &operator=(const EventPoller &) = delete;

	[[nodiscard]] virtual std::optional<Event> nextEvent() = 0;
};

} // namespace dust::platform
