#include "Engine.h"

int main(){
	Debug::Logger::Instance().minLogLevel = Debug::Logger::Level::SPAM;

	Engine engine;

	Engine::EngineConfig conf;
	conf.windowMode = Render::WindowMode::Windowed;
	
	engine.Run(conf);

	return 0;
}