#include <dust/backend/sdl/event_poller.hpp>
#include <dust/backend/sdl/lib.hpp>
#include <dust/backend/sdl/window.hpp>
#include <dust/game/game_loop.hpp>

int main(int argc, const char **argv)
{
	using namespace dust::backend;
	using namespace dust::game;

	auto &sdl = sdl::Lib::instance();

	return GameLoop().start(
		std::make_shared<sdl::EventPoller>(),
		std::make_shared<sdl::Window>("Dust Game", std::make_tuple(1280, 720)));
}
