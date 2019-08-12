#pragma once
#include "SDL.h"
using namespace std;

class Game
{
private:
	bool m_bStarted, m_bRunning;
	bool m_bGotTick;
	Uint32 m_start, m_end, m_delta, m_fps;

	Game();
	bool Init();
	bool Running();
	void Update();
	void HandleEvents();
	void Wake();
	void Sleep();
	void Render();
	void Clean();

public:
	static Game* Instance();
	void Quit() { m_bRunning = false; }
	void Run();
};