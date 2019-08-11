#include "Game.h"

int main(int argc, char* args[])
{
	Game::Instance()->Run("Gosh dang to Heck!: A profanity-free game...", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, 1024, 768, 0);
	return 0;
}