#include "stdafx.h"
#include "RulePool.h"
#include "SDL_Manager.h"
#include "MainHolder.h"
#include "Camera.h"
#include "Building.h"
#include "Floorplan.h"
#include "Map.h"
#include "Skybox.h"
#include "Floor.h"
#include "Axis.h"
#include "Logger.h"
#include "Utilities.h"
#include "Config.h"

int main() {
	Logger logger;
	logger.log("Start");

	/*** SETUP ***/

	Config::parseConfig("Config.xml");

	SDL_Manager sdl;
	sdl.initialize();

	MainHolder mainHolder;
	mainHolder.initialize();
	mainHolder.getMaterialHolder().parseMaterials(Config::materialsFile);
	mainHolder.getShapeInfoHolder().parseShapeInfos(Config::shapeInfoFile);

	RulePool rulepool = RulePool();
	rulepool.parseRulepool(Config::rulepoolFile);

	Camera camera = Camera();

	logger.updateAndLogDuration("Setup Finished");

	/*** LOAD MAP ***/
	
	Map map;
	map.parseMap(Config::mapToParseFile, Config::rectanglize);
	/*Shape church = Shape("Church");
	church.resize(35, 25, 50);
	church.translate(-50, 0, -200);
	map.addBuilding(Building(church));*/

	logger.updateAndLogDuration("Parse Finished");

	/*** GROW ***/

	map.growAll(rulepool, Config::lod);

	logger.updateAndLogDuration("Growth Finished");

	/*** BUILD DRAWABLE ***/

	GLuint program = mainHolder.getProgramHolder().getProgram("singleColor3D");
	ShapeInfoHolder shapeInfoHolder = mainHolder.getShapeInfoHolder();

	map.buildDrawable(program, shapeInfoHolder);

	logger.updateAndLogDuration("Drawables Finished");

	/*** SAVE MODEL ***/

	struct std::tm newtime;
	std::time_t now = std::time(NULL);
	localtime_s(&newtime, &now);
    char timestamp[32];
	std::strftime(timestamp, sizeof(timestamp), "--%F--%H-%M", &newtime);

	std::string obj = Config::modelFile + timestamp + ".obj";
	std::string mtl = Config::modelFile + timestamp + ".mtl";

	std::ofstream myfile;

	myfile.open(obj);
	myfile << map.toOBJ(mtl);
	myfile.close();

	myfile.open(mtl);
	myfile << mainHolder.getMaterialHolder().toMTL();
	myfile.close();

	logger.updateAndLogDuration("Model Saved");

	/*** LOOP DRAW ***/

	Skybox skybox = Skybox(program);
	Floor floor = Floor(program);

	Map::prepareDraw(mainHolder.getProgramHolder());
	GLuint uniProjection = glGetUniformLocation(program, "projection");
	GLuint uniView = glGetUniformLocation(program, "view");
	GLuint uniModel = glGetUniformLocation(program, "model");
	GLuint uniSunDirection = glGetUniformLocation(program, "sunDirection");
	GLuint uniColor = glGetUniformLocation(program, "color");
	glUniform1i(glGetUniformLocation(program, "tex"), 0);

	//Light
	glm::vec3 sunDirection = Config::sunDirection;
	glUniform3f(uniSunDirection, sunDirection.x, sunDirection.y, sunDirection.z);

	//Loop
	SDL_Event event;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	bool quit = false;
	int mouseX, mouseY;
	auto lastTime = std::chrono::high_resolution_clock::now();
	while (!quit) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float, std::ratio<1>> timeElapsed = currentTime - lastTime;
		lastTime = currentTime;

		//Mouse movement
		SDL_GetRelativeMouseState(&mouseX, &mouseY);

		//Handle single events
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				quit = true;
			} else if (event.type == SDL_KEYUP) {
				if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) {
					quit = true;
				}
			} else if (event.type == SDL_MOUSEMOTION) {
				camera.rotate((float)mouseX, (float)mouseY);
			}
		}

		int movementForward = 0;
		int movementSideways = 0;
		if (keys[SDL_SCANCODE_W]) {
			movementForward++;
		}
		if (keys[SDL_SCANCODE_S]) {
			movementForward--;
		}
		if (keys[SDL_SCANCODE_A]) {
			movementSideways--;
		}
		if (keys[SDL_SCANCODE_D]) {
			movementSideways++;
		}
		if (movementForward != 0 || movementSideways != 0) {
			float angle = (float) glm::degrees(atan2(movementForward, movementSideways)) - 90.0f;
			camera.move(-angle, timeElapsed.count());
		}

		if (keys[SDL_SCANCODE_SPACE]) {
			camera.climb(timeElapsed.count());
		}
		if (keys[SDL_SCANCODE_LSHIFT]) {
			camera.fall(timeElapsed.count());
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = camera.getProjection();
		glUniformMatrix4fv(uniProjection, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 view = camera.getView();
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 model = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

		map.draw(mainHolder.getMaterialHolder(), uniColor);

		view[3] = { std::fmod(view[3].x, FLOOR_DIVIDER), view[3].y, std::fmod(view[3].z, FLOOR_DIVIDER), 1 };
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
		floor.draw(mainHolder.getMaterialHolder(), uniColor);

		view[3] = { 0,0,0,1 };
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
		skybox.draw(mainHolder.getMaterialHolder(), uniColor);

		sdl.swap();
	}

	/*** QUIT ***/

	map.quit();
	skybox.quit();
	floor.quit();
	mainHolder.quit();

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cout << "OpenGL error: " << error << std::endl;
	}
	
	sdl.quit();

    return 0;
}

