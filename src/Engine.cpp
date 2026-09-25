#include "Engine.h"

#include <iostream>

Engine *Engine::instance = nullptr;

Engine::Engine()
{
	Engine::instance = this;
}

Engine::~Engine()
{
	Engine::instance = nullptr;
}

void Engine::Run(EngineConfig &config)
{
	while (run)
	{
		std::cout << "Running!" << std::endl;
	}
}
