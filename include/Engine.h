#pragma once

#include "debug/Logger.h"
#include "rendering/OpenGL/GLRenderer.h"

class Engine{
public:
	struct EngineConfig {
			Render::WindowMode windowMode = Render::WindowMode::Windowed;
			int windowHeight = 800;
			int windowWidth  = 600;
	};

	static Engine *instance;
	Render::IRenderer *renderer = nullptr;

	Engine();
	~Engine();

	void Run(EngineConfig& config);
private:
};