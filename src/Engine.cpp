/** Penoxy
 *
 * Copyright (C) 2026 Penoxy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 *
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "Engine.h"

Engine *Engine::instance = nullptr;

Engine::Engine() {
	Engine::instance = this;
}

Engine::~Engine() {
	Debug::LogTrace("Engine destructor triggered");

	delete this->renderer;
	this->renderer = nullptr;

	Engine::instance = nullptr;
}

void Engine::Run(EngineConfig &config) {
	this->renderer = new Render::GLRenderer(config.windowMode, {config.windowHeight, config.windowWidth});

	while (!this->renderer->ShouldClose()) {
		// TODO: temporary for testing
		char32_t input;
		Render::IWindow *w = this->renderer->GetWindow();
		while (w->LoadCharFromQueue(&input)){
			if(IsSpecialUTF32Char(input)) Debug::LogInfo("Special key pressed!");
			else{
				std::string msg{(char)input};
				msg = "Logged input: " + msg;
				Debug::LogInfo(msg);
			}
		}
		// --------------
		

		this->renderer->Render();
	}
}
