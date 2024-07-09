#pragma once

#include <android_native_app_glue.h>

#include <dust/platform/event_poller.hpp>

namespace dust::android
{

class EventPoller final : public dust::platform::EventPoller
{
public:
	explicit EventPoller(android_app *app);

	std::optional<platform::Event> nextEvent() final;

private:
	android_app *m_app;
};

} // namespace dust::android
