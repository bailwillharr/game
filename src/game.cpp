#include "config.h"

#include "engine.hpp"

void playGame()
{
	engine::Application app(PROJECT_NAME, PROJECT_VERSION);

	app.gameLoop();

}