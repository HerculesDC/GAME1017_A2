#include <chrono>
#include <iostream>

#include "Game.h"
#include "Managers.hpp"
#include "MessageFactory.hpp"
#include "StateMachine.hpp"
#include "States.hpp"

#define FPS 60
using namespace std;
using namespace chrono;

Game::Game() :m_bStarted(false), m_bRunning(false)
{
	cout << "Constructing engine." << endl;
	m_fps = (Uint32)round((1 / (double)FPS) * 1000); // Sets FPS in milliseconds and rounds.
}

Game* Game::Instance()
{
	static Game* instance = new Game();
	return instance;
}

void Game::Run(const char * title, int xPos, int yPos, int width, int height, int flags)
{
	if (m_bStarted == true)
		return;
	cout << "Starting game." << endl;
	m_bStarted = true;
	if (Init(title, xPos, yPos, width, height, flags) == false)
	{
		cout << "Cannot initialize game." << endl;
		return;
	}
	// Main engine loop here.
	while (Running())
	{
		Wake();
		HandleEvents(); // Not restricted to framerate.
		Update();
		Render();
		if (Running())
			Sleep();
	}
	// End main engine loop.
	Clean();
}

bool Game::Init(const char* title, const int xpos, const int ypos, 
			    const int width, const int height, const int flags)
{
	// Attempt to initialize SDL.
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) // 0 is error code meaning success.
	{
		cout << "SDL init success!" << endl;
		// Initialize the window
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (m_pWindow != nullptr) // Window init success. 
		{
			cout << "Window creation successful!" << endl;
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
			if (m_pRenderer != nullptr) // Renderer init success. 
			{
				SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
				cout << "Renderer creation success!" << endl;
				if (TextureManager::Instance()->Init())
				{
					//may require refactoring
					TextureManager::Instance()->Add("Img/TitleScreen.png");
					TextureManager::Instance()->Add("Img/playButton.png");
					TextureManager::Instance()->Add("Img/Backgrounds.png");
					TextureManager::Instance()->Add("Img/Obstacles.png");
					TextureManager::Instance()->Add("Img/Player.png");
				}
				else
				{
					cout << "Image init fail!" << endl;
					return false;// Image init fail.
				}
			}
			else
			{
				cout << "Renderer init fail!" << endl;
				return false; // Renderer init fail. 
			}
		}
		else
		{
			cout << "Window init fail!" << endl;
			return false; // Window init fail. 
		}

		if (AudioManager::Instance()->Init()) {
			AudioManager::Instance()->AddMusic("Sound/Chase.mp3");
			AudioManager::Instance()->AddMusic("Sound/Death.mp3");
			AudioManager::Instance()->AddChunk("Sound/SCD_FM_02.wav");
		}
		else
		{
			cout << "Sound init fail!" << endl;
			return false; // SDL init fail. 
		}

		if (MessageFactory::Instance()->Init())
			TextureManager::Instance()->Add(MessageFactory::Instance()->Export("GOSH  DANG  TO  HECK !"));
		else cout << "fonts not initialized" << endl;

		StateMachine::Instance()->RequestStateChange(new TitleState());
	}
	else
	{
		cout << "SDL init fail!" << endl;
		return false; // SDL init fail. 
	}

	
	srand((unsigned)time(NULL));
	m_iKeystates = SDL_GetKeyboardState(nullptr);
	m_bRunning = true;
	return true;
}

bool Game::Running()
{
	return m_bRunning;
}

bool Game::KeyDown(SDL_Scancode c)
{
	if (m_iKeystates != nullptr)
	{
		if (m_iKeystates[c] == 1)
			return true;
		else
			return false;
	}
	return false;
}

void Game::Update()
{
	StateMachine::Instance()->Update();	
}

void Game::HandleEvents()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_bRunning = false;
			break;
		}
	}
}

void Game::Wake()
{
	m_start = SDL_GetTicks();
}

void Game::Sleep()
{
	m_end = SDL_GetTicks();
	m_delta = m_end - m_start;
	if (m_delta < m_fps) // Engine has to sleep.
		SDL_Delay(m_fps - m_delta);
}

void Game::Render()
{
	StateMachine::Instance()->Render();
}

void Game::Clean()
{
	cout << "Cleaning game. Bye!" << endl;
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	IMG_Quit();
	SDL_Quit();
}