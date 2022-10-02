#include <config.h>

#include <log.hpp>

#include "camera_controller.hpp"
#include "terrain.hpp"
#include "meshgen.hpp"

#include "window.hpp"
#include "input.hpp"
#include "resource_manager.hpp"
#include "sceneroot.hpp"

#include "components/text_ui_renderer.hpp"
#include "components/mesh_renderer.hpp"
#include "components/camera.hpp"

#include "gfx_device.hpp"

static void addInputs(engine::Input& input);
static void gameLoop(engine::Window& win, engine::Input& input, ResourceManager& resMan);

void playGame()
{

	engine::Window win(PROJECT_NAME);
	engine::GFXDevice dev(engine::AppInfo{PROJECT_NAME, PROJECT_VERSION}, win.m_handle);

	engine::Input input(win); // Input Manager
	ResourceManager resMan;

	gameLoop(win, input, resMan);

}

static void addInputs(engine::Input& input)
{
	// game buttons
	input.addInputButton("fire", inputs::MouseButton::M_LEFT);
	input.addInputButton("aim", inputs::MouseButton::M_RIGHT);
	input.addInputButton("jump", inputs::Key::SPACE);
	input.addInputButton("sneak", inputs::Key::LSHIFT);
	// game movement
	input.addInputButtonAsAxis("movex", inputs::Key::D, inputs::Key::A);
	input.addInputButtonAsAxis("movey", inputs::Key::W, inputs::Key::S);
	// looking around
	input.addInputAxis("lookx", inputs::MouseAxis::X);
	input.addInputAxis("looky", inputs::MouseAxis::Y);
}

static void gameLoop(engine::Window& win, engine::Input& input, ResourceManager& resMan)
{

	win.setRelativeMouseMode(false);

	uint64_t lastTick = win.getNanos();
	uint64_t lastLongTick = lastTick;

	constexpr int TICKFREQ = 20; // in Hz
	
	// single-threaded game loop
	while (win.isRunning()) {

		if (win.getLastFrameStamp() >= lastTick + (BILLION / TICKFREQ)) {
			// every 20th of a second
			lastTick = win.getLastFrameStamp();
		}
		if (win.getLastFrameStamp() >= lastLongTick + (BILLION * 5)) {
			// every 5 seconds
			lastLongTick = win.getLastFrameStamp();
			win.resetAvgFPS();
		}

		// logic

		if (win.getKeyPress(inputs::Key::F11)) {
			if (win.isFullscreen()) {
				win.setFullscreen(false);
			}
			else {
				win.setFullscreen(true, false); // disable exclusive mode, use borderless window
				//win.setFullscreen(true, true); // use exclusive fullscreen
			}
		}
		if (win.getKeyPress(inputs::Key::Q)) {
			bool captured = win.mouseCaptured();
			win.setRelativeMouseMode(!captured);
			input.setDeviceActive(engine::InputDevice::MOUSE, !captured);
		}
		if (win.getKeyPress(inputs::Key::TAB)) {
			win.infoBox("RESOURCES", resMan.getResourcesListString()->c_str());
		}

		if (win.getKeyPress(inputs::Key::ESCAPE)) {
			win.setCloseFlag();
		}

		// events
		win.getInputAndEvents();

	}

}
