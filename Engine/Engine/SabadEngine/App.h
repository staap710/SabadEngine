#pragma once
namespace SabadEngine
{
	class App final {
	public:
		void Run();
		void Quit();
	private:
		bool mRunning = false
	};

}