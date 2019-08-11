#include <chrono>
#include <iostream>

#include "Game.h"
#include "Managers.hpp"
#include "MessageFactory.hpp"
#include "StateMachine.hpp"
#include "States.hpp"
#include "CommandHandler.hpp"

#define FPS 60 //60 causes flickering
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
					TextureManager::Instance()->Add("Img/TitleScreen.png");	//0
					TextureManager::Instance()->Add("Img/playButton.png");	//1
					TextureManager::Instance()->Add("Img/quitButton.png");	//2
					TextureManager::Instance()->Add("Img/Backgrounds.png"); //3
					TextureManager::Instance()->Add("Img/Obstacles.png");	//4
					TextureManager::Instance()->Add("Img/Player.png");		//5
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

		if (MessageFactory::Instance()->Init()) {
			//I planned to use a simple file parser for this, but it would introduce complications when indexing.
			//I opted for this mostly for simplicity
			//OBS: lowercase: full font <=> UPPERCASE: outline font											//Texture index
			TextureManager::Instance()->Add(MessageFactory::Instance()->Export("gosh  dang  to  heck !"));	//6
			TextureManager::Instance()->Add(MessageFactory::Instance()->Export("A  PROFANITY FREE  GAME"));	//7
			TextureManager::Instance()->Add(MessageFactory::Instance()->Export("play"));					//8
			TextureManager::Instance()->Add(MessageFactory::Instance()->Export("quit"));					//9
			TextureManager::Instance()->Add(MessageFactory::Instance()->Export("CHARACTER  SELECT"));		//10
			TextureManager::Instance()->Add(MessageFactory::Instance()->Export("back"));					//11
			TextureManager::Instance()->Add(MessageFactory::Instance()->Export("GAME  PAUSED"));			//12
			TextureManager::Instance()->Add(MessageFactory::Instance()->Export("resume"));					//13
			TextureManager::Instance()->Add(MessageFactory::Instance()->Export("u  dead !"));				//14
			TextureManager::Instance()->Add(MessageFactory::Instance()->Export("retry ?"));					//15
			TextureManager::Instance()->Add(MessageFactory::Instance()->Export("title"));					//16
		}
		else { 
			
			cout << "fonts not initialized" << endl;
			return false;
		}

		StateMachine::Instance().RequestStateChange(new MachineStates(TITLE));
	}
	else
	{
		cout << "SDL init fail!" << endl;
		return false; // SDL init fail. 
	}

	
	srand((unsigned)time(NULL));
	
	m_bRunning = true;
	return true;
}

bool Game::Running()
{
	return m_bRunning;
}

void Game::Update()
{
	StateMachine::Instance().Update();	
}

void Game::HandleEvents()
{
	CommandHandler::Instance()->HandleEvents();
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
	StateMachine::Instance().Render();
}

void Game::Clean()
{
	cout << "Cleaning game. Bye!" << endl;
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	IMG_Quit();
	SDL_Quit();
}