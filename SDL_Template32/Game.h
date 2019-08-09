#pragma once
#include "SDL.h"
using namespace std;

class Game
{
private:
	bool m_bStarted, m_bRunning;
	bool m_bGotTick;
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	const Uint8* m_iKeystates;
	Uint32 m_start, m_end, m_delta, m_fps;

	// Create (number visible +1) objects for each background.

	Game();
	bool Init(const char* title, const int xpos, const int ypos,
		const int width, const int height, const int flags);
	bool Running();
	void Update();
	void HandleEvents();
	void Wake();
	void Sleep();
	void Render();
	void Clean();

public:
	static Game* Instance();
	void Run(const char*, int, int, int, int, int);
	SDL_Window* GetWindow() { return m_pWindow; }
	SDL_Renderer* GetRenderer() { return m_pRenderer; }
	bool KeyDown(SDL_Scancode c);
};