#include "config.h"

#include <logger.hpp>

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
#ifdef NDEBUG
		Window::errorBox(e.what());
#else
		fputs(e.what(), stderr);
		fputc('\n', stderr);
#endif
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
