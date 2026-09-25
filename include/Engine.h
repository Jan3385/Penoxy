#pragma once

class Engine{
	public:
		struct EngineConfig{
				// ...
		};

		static Engine *instance;

		Engine();
		~Engine();

		void Run(EngineConfig& config);
	private:
		bool run = true;
};