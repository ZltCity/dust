#include "event_poller.hpp"

namespace dust::android
{

EventPoller::EventPoller(android_app *app) : m_app(app)
{
}

std::optional<platform::Event> EventPoller::nextEvent()
{
	int events;
	struct android_poll_source *source;

	if (ALooper_pollOnce(0, nullptr, &events, (void **)&source) >= 0)
	{
		if (source != nullptr)
		{
			source->process(m_app, source);
		}

		if (m_app->destroyRequested != 0)
		{
			return platform::Event {.type = platform::Event::Type::Quit};
		}
		else
		{
			return platform::Event {};
		}
	}

	return std::nullopt;
}

} // namespace dust::android
