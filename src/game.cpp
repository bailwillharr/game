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

using namespace components;

class Spin : public CustomComponent {
public:
	float speed = 0.2f;
	float pitch = 0.0f;

	Spin(Object* parent) : CustomComponent(parent) {}

	void onUpdate(glm::mat4) override
	{
		float dt = parent.win.dt();
		yaw += dt * speed;
		yaw = glm::mod(yaw, glm::two_pi<float>());
		parent.transform.rotation = glm::angleAxis(yaw, glm::vec3{0.0f, 1.0f, 0.0f});
		parent.transform.rotation *= glm::angleAxis(pitch, glm::vec3{1.0f, 0.0f, 0.0f});
	}
private:
	float yaw = 0.0f;
};

static void addObjects(SceneRoot& mainScene);
static void addInputs(Input& input);
static void gameLoop(Window& win, Input& input, ResourceManager& resMan, SceneRoot& menu, SceneRoot& mainScene);
static void createMenu(SceneRoot& menu);

void playGame()
{

	Window win(PROJECT_NAME);
	engine::gfx::Device dev(engine::AppInfo{PROJECT_NAME, PROJECT_VERSION}, win);

/*
	Input input(win); // Input Manager
	ResourceManager resMan;

	SceneRoot menu({ &win, &input, &resMan });
	SceneRoot mainScene({ &win, &input, &resMan });
	addInputs(input);
	createMenu(menu);
	addObjects(mainScene);

	gameLoop(win, input, resMan, menu, mainScene);
*/

}

static void createMenu(SceneRoot& menu)
{
	
	// pause menu
	menu.createChild("cam")->createComponent<Camera>()->useOrtho();
	menu.createChild("text")->createComponent<UI>()->m_text = "PAUSED";
	menu.getChild("text")->getComponent<UI>()->m_color = {1.0f, 0.0f, 0.0f};
	menu.getChild("text")->transform.position = {-1.0f, 0.90f, 0.0f};
	menu.createChild("text2")->createComponent<UI>()->m_text = "Press enter to play";
	menu.getChild("text2")->transform.position = {-1.0f, 0.70f, 0.0f};
	menu.createChild("text3")->createComponent<UI>()->m_text = "Press escape to quit";
	menu.getChild("text3")->transform.position = {-1.0f, 0.50f, 0.0f};

	menu.createChild("donut")->createComponent<Renderer>()->setMesh("meshes/donut.mesh");
	menu.getChild("donut")->transform.position = {0.0f, 0.0f, -5.0f};
	menu.getChild("donut")->transform.scale = {0.025f, 0.025f, 0.025f};
	menu.getChild("donut")->transform.rotation = glm::angleAxis(glm::half_pi<float>(), glm::vec3{1.0f, 0.0f, 0.0f});

	menu.getChild("donut")->createComponent<Spin>()->pitch = glm::half_pi<float>();
	menu.getChild("donut")->getComponent<Spin>()->speed = 4.0f;
}

static void addObjects(SceneRoot& mainScene)
{

	// MISC OBJECTS

/*
	auto chart = mainScene.createChild("chart");
	auto chartTransform = chart->getComponent<Transform>();
	chartTransform->position = { 0.0f, 2.0f, -5.0f };
	chartTransform->scale = { 2.0f, 2.0f, 1.0f };
	auto chartRenderer = chart->createComponent<Renderer>();
	chartRenderer->m_mesh = std::make_unique<resources::Mesh>(std::vector<Vertex>{
		{ { 1.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
		{ { -1.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f  } },
		{ { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
		{ { 1.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
		{ { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f  } },
		{ { 1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } }
	});
	chartRenderer->m_texture = std::make_unique<resources::Texture>(true);
	chartRenderer->m_emission = { 1.0f, 1.0f, 1.0f };
*/

	// CAMERA

	auto cam = mainScene.createChild("cam");
	constexpr float HEIGHT_INCHES = 6.0f * 12.0f;
	// eye level is about 4 1/2 inches below height
	constexpr float EYE_LEVEL = (HEIGHT_INCHES - 4.5f) * 25.4f / 1000.0f;
	cam->transform.position = { 0.0f, EYE_LEVEL, 0.0f };
	auto camCamera = cam->createComponent<Camera>();
	camCamera->usePerspective(70.0f);
	cam->createComponent<CameraController>();
	camCamera->m_noClear = false;
	camCamera->clearColor = { 0.5f, 0.5f, 0.5f, 1.f };
	cam->createComponent<Renderer>()->m_mesh = genSphereMesh(0.2f, 20);
	cam->getComponent<Renderer>()->setTexture("textures/cobble_stone.png");

	auto gun = cam->createChild("gun");
	gun->transform.position = glm::vec3{0.2f, -0.1f, -0.15f};
	gun->transform.rotation = glm::angleAxis(glm::pi<float>(), glm::vec3{ 0.0f, 1.0f, 0.0f });
	float GUN_SCALE = 9.0f / 560.0f;
	GUN_SCALE *= 1.0f;
	gun->transform.scale *= GUN_SCALE;
	auto gunRenderer = gun->createComponent<Renderer>();
	gunRenderer->setMesh("meshes/gun.mesh");
	gunRenderer->setTexture("textures/gun.png");



	// USER INTERFACE

	class HUDLogic : public CustomComponent {
	public:
		UI* ui;
		uint64_t lastAction;

		UI* hudPosText;
		Object* playerCam = nullptr;

		HUDLogic(Object* parent) : CustomComponent(parent)
		{
			ui = parent->getComponent<UI>();
			parent->transform.position.x = 1.0f;
			parent->transform.position.y = 0.95f;
			parent->transform.scale = {0.5f, 0.5f, 1.0f};
			ui->m_alignment = components::UI::Alignment::RIGHT;

			lastAction = win.getNanos();

			playerCam = parent->getParent()->getChild("cam");

			hudPosText = parent->getParent()->getChild("position_text")->getComponent<UI>();
			hudPosText->m_alignment = components::UI::Alignment::LEFT;
		}

		void onUpdate(glm::mat4) override
		{
			

			constexpr float DELAY = 0.02f;
			const uint64_t now = win.getNanos();
			if (now - lastAction > DELAY * BILLION) {
				lastAction = now;
				//win.setTitle(std::to_string(win.getFPS()));
				ui->m_text = "FPS: " + std::to_string(win.getFPS());
				ui->m_text += " " + std::to_string(win.dt() * 1000.0f) + " ms";

				const std::string positionText =
					"x: " + std::to_string(playerCam->transform.position.x) +
					" y: " + std::to_string(playerCam->transform.position.y) +
					" z: " + std::to_string(playerCam->transform.position.z);

				hudPosText->m_text = positionText;
			}
		}
	};

	auto hud = mainScene.createChild("hud");
	hud->createComponent<UI>();
	auto hudPosText = mainScene.createChild("position_text");
	auto hudPosTextUI = hudPosText->createComponent<UI>();
	hudPosText->transform.position = { -1.0f, 0.95f, 0.0f };
	hudPosText->transform.scale = { 0.5f, 0.5f, 1.0f };
	hud->createComponent<HUDLogic>();



	// FLOOR
	
	constexpr float GRASS_DENSITY = 128.0f * 20.0f;
	auto floor = mainScene.createChild("floor");
	auto floorRenderer = floor->createComponent<Renderer>();
	floor->transform.position = glm::vec3{ 0.0f, 0.0f, 0.0f };
	floorRenderer->setTexture("textures/stone_bricks.png");
	floorRenderer->m_mesh = std::make_unique<resources::Mesh>(std::vector<Vertex>{
		{ { -16.0f, 0.0f, 16.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f,  GRASS_DENSITY } },
		{ {  16.0f, 0.0f, -16.0f }, { 0.0f, 1.0f, 0.0f }, { GRASS_DENSITY, 0.0f } },
		{ { -16.0f, 0.0f, -16.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f,  0.0f  } },
		
		{ {  16.0f, 0.0f,  16.0f }, { 0.0f, 1.0f, 0.0f }, { GRASS_DENSITY, GRASS_DENSITY } },
		{ {  16.0f, 0.0f, -16.0f }, { 0.0f, 1.0f, 0.0f }, { GRASS_DENSITY, 0.0f  } },
		{ { -16.0f, 0.0f,  16.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f,  GRASS_DENSITY } },	
		
	});
	floor->transform.scale = { 100.0f, 1.0f, 100.0f };



	// BOX
	
	auto boundary = mainScene.createChild("boundary");
	auto boundaryRen = boundary->createComponent<Renderer>();
	boundaryRen->m_mesh = genSphereMesh(100.0f, 32, true);
	boundaryRen->setTexture("textures/white.png");
	boundaryRen->m_color = {0.8f, 0.2f, 0.03f};


	// WORLD GEN

	auto chunk = mainScene.createChild("chunk");
	chunk->createComponent<Renderer>()->m_mesh = getChunkMesh(0, 0);
	chunk->transform.position = {8.0f, 1.0f, 8.0f};






	auto monke = mainScene.createChild("monke");
	auto monkeRen = monke->createComponent<Renderer>();
	
	monkeRen->setMesh("meshes/monke.mesh");
	monkeRen->setTexture("textures/MonkeTexture.png");
	monkeRen->m_emission = { 0.05f, 0.05f, 0.05f };

	monke->transform.position = { -10.0f, 1.0f, 10.0f };



	auto earth = mainScene.createChild("earth");
	earth->transform.position = { 0.0f, 7.0f, -15.0f };
	auto earthRenderer = earth->createComponent<Renderer>();
	earthRenderer->setTexture("textures/grass_block_top.png");
	earthRenderer->m_color = { 145.0f/255.0f, 189.0f/255.0f, 89/255.0f };
	earth->createComponent<Spin>();
	mainScene.createChild("detail_text")->createComponent<UI>();
	mainScene.getChild("detail_text")->transform.position = { 0.0f, -0.95f, 0.0f };
	mainScene.getChild("detail_text")->transform.scale = { 0.5f, 0.5f, 1.0f };

	class DetailAdjust : public CustomComponent {
	public:
		DetailAdjust(Object* parent) : CustomComponent(parent)
		{
			r = parent->getComponent<Renderer>();
			text = parent->getParent()->getChild("detail_text")->getComponent<UI>();
			setText();
			lastAction = win.getNanos();
		}

		void onUpdate(glm::mat4) override
		{
			if (win.getKeyPress(inputs::Key::EQUALS)) {
				detail += step;
				regenSphere();
				setText();
			} else if (win.getKeyPress(inputs::Key::MINUS)) {
				detail -= step;
				if (detail < 3) detail = 3;
				regenSphere();
				setText();
			}

			constexpr float DELAY = 0.1f;
			const uint64_t now = win.getNanos();
			if (now - lastAction > DELAY * BILLION) {
				lastAction = now;
				if (win.getKey(inputs::Key::N0)) {
					step += 1;
					setText();
				}
				else if (win.getKey(inputs::Key::N9)) {
					step -= 1;
					if (step < 1) step = 1;
					setText();
				}
			}

		}
	private:
		int detail = 10;
		int step = 1;

		uint64_t lastAction;

		Renderer* r;
		UI* text;

		void regenSphere()
		{
			r->m_mesh = genSphereMesh(6.0f, detail);
		}

		void setText()
		{
			text->m_text = "DETAIL: " + std::to_string(detail) + " STEP: " + std::to_string(step);
		}
	};

	earth->createComponent<DetailAdjust>();


	auto cuboidRen = mainScene.createChild("cuboid")->createComponent<Renderer>();
	cuboidRen->setMesh("meshes/cube.mesh");
	cuboidRen->setTexture("textures/cobble_stone.png");
	mainScene.getChild("cuboid")->transform.scale = {0.1f, 5.0f, 5.0f};
	mainScene.getChild("cuboid")->transform.position = {-65.0f, 5.0f, 0.0f};

	auto sunPivot = mainScene.createChild("sun_pivot");
	sunPivot->transform.position = { 0.0f, 10.0f, 0.0f };
	auto sun = sunPivot->createChild("sun");
	sun->transform.position = { 50.0f, 0.0f, 0.0f };
	auto sunRen = sun->createComponent<Renderer>();
	sunRen->m_mesh = genSphereMesh(2.0f, 16);
	sunRen->m_color = { 1.0f, 1.0f, 1.0f };
	sunRen->m_emission = { 1.0f, 1.0f, 1.0f };
	sunRen->setTexture("textures/white.png");
	class SunMovement : public CustomComponent {
	public:
		SunMovement(Object* parent) : CustomComponent(parent)
		{
		}

		void onUpdate(glm::mat4 t) override
		{
			float dt = parent.win.dt();

			glm::vec3 posWorldSpace = { t[3][0], t[3][1], t[3][2] };
			parent.root.getChild("cam")->getComponent<Camera>()->m_lightPos = posWorldSpace;
			
			yaw += dt * speed;
			yaw = glm::mod(yaw, glm::two_pi<float>());
			parent.getParent()->transform.rotation = glm::angleAxis(yaw, glm::vec3{0.0f, 1.0f, 0.0f});
		}
	private:
		float yaw = 0.0f;
		float speed = 1.0f;
	};
	sun->createComponent<SunMovement>();

	mainScene.printTree();

}

static void addInputs(Input& input)
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

static void gameLoop(Window& win, Input& input, ResourceManager& resMan, SceneRoot& menu, SceneRoot& mainScene)
{

//	win.setVSync(true);
	win.setRelativeMouseMode(false);

	uint64_t lastTick = win.getNanos();
	uint64_t lastLongTick = lastTick;

	constexpr int TICKFREQ = 20; // in Hz
	
	SceneRoot *activeScene = &menu;
	
	// single-threaded game loop
	while (win.isRunning()) {

		if (win.getLastFrameStamp() >= lastTick + (BILLION / TICKFREQ)) {
			lastTick = win.getLastFrameStamp();
		}
		if (win.getLastFrameStamp() >= lastLongTick + (BILLION * 5)) {
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
			input.setDeviceActive(InputDevice::MOUSE, !captured);
		}
		if (win.getKeyPress(inputs::Key::TAB)) {
			win.infoBox("RESOURCES", resMan.getResourcesListString()->c_str());
		}
/*		if (win.getKeyPress(inputs::Key::V)) {
			win.setVSync(!win.getVSync());
		}
*/




		if (activeScene == &menu) {

			if (win.getKeyPress(inputs::Key::ESCAPE)) {
				win.setCloseFlag();
			}

			// In the menu
			if (win.getKeyPress(inputs::Key::RETURN)) {
				win.setResizedFlag();
				win.setRelativeMouseMode(true);
				activeScene = &mainScene;
			}

		} else {

			// Playing game

			if (win.getKeyPress(inputs::Key::N3)) {
				mainScene.getChild("cam")->getComponent<components::Camera>()->usePerspective(70.0f);
			}
			if (win.getKeyPress(inputs::Key::N2)) {
				mainScene.getChild("cam")->getComponent<components::Camera>()->useOrtho();
			}
			
			if (win.getKeyPress(inputs::Key::ESCAPE)) {
				// Pause game
				win.setResizedFlag();
				win.setRelativeMouseMode(false);
				activeScene = &menu;
			}

		}

		activeScene->updateStuff();
		
		// events
		win.getInputAndEvents();

	}

}
