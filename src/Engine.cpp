#include "Engine.h"

Engine *Engine::instance = nullptr;

Engine::Engine() {
	Engine::instance = this;
}

Engine::~Engine() {
	Engine::instance = nullptr;
}

void Engine::Run(EngineConfig &config) {
	this->renderer = new Render::GLRenderer(config.windowMode, {config.windowHeight, config.windowWidth});

	while (!this->renderer->ShouldClose()) {
		// TODO: temporary spot. move later
		glfwPollEvents();

		this->renderer->Render();
	}
}
