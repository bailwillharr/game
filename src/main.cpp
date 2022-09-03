#include "config.h"

#include <engine.hpp>

#include "game.hpp"

#include <exception>
#include <window.hpp>

int main(int argc, char *argv[])
{

	engine::setupLog(PROJECT_NAME);

	INFO("{} v{}", PROJECT_NAME, PROJECT_VERSION);

	try {
		playGame();
	}
	catch (const std::exception& e) {
		Window::errorBox(e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
