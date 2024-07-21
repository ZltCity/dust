#include <dust/game/game_loop.hpp>
#include <dust/sdl/sdl.hpp>

int main(int argc, const char **argv)
{
	using namespace dust;

	auto &sdl = sdl::Lib::instance();

	return game::GameLoop().start(std::make_shared<sdl::Window>("Dust Game", std::make_tuple(1280, 720)));
}
