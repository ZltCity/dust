#include "game/game.hpp"

int main(int argc, char **argv)
{
	using Game = dust::game::Game;

	return Game(argc, argv).start();
}