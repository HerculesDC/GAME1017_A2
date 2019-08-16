#include <chrono>
#include <iostream>

#include "Game.h"
#include "Managers.hpp"
#include "MessageFactory.hpp"
#include "StateMachine.hpp"
#include "States.hpp"
#include "CommandHandler.hpp"

#define FPS 30 //60 causes flickering during pause, probably due to update/render times
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

void Game::Run()
{
	if (m_bStarted == true) return;
	m_bStarted = true;
	if (!Init()) return; //Game cannot be initialized
	
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

bool Game::Init()
{
	// Attempt to initialize SDL.
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) // 0 is error code meaning success.
	{	
		if (WindowManager::Instance()->GetWindow() != nullptr) // Window init success. 
		{
			cout << "Window and its manager initialized successfully!" << endl;
			if (RendererManager::Instance()->GetRenderer() != nullptr) // Renderer init success. 
			{
				cout << "Renderer and its manager initialized successfully!" << endl;
				SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
				if (TextureManager::Instance()->Init())
				{
					//may require refactoring
					TextureManager::Instance()->Add("Img/TitleScreen.png");	//0
					TextureManager::Instance()->Add("Img/MenuScreen.png"); 	//1
					TextureManager::Instance()->Add("Img/playButton.png");	//2
					TextureManager::Instance()->Add("Img/quitButton.png");	//3
					TextureManager::Instance()->Add("Img/upButton.png");	//4
					TextureManager::Instance()->Add("Img/Backgrounds.png");	//5
					TextureManager::Instance()->Add("Img/Obstacles.png");	//6
					TextureManager::Instance()->Add("Img/Player.png");		//7
				}
				else
				{
					cout << "Image init fail!" << endl;
					return false;// Image init fail.
				}

				if (MessageFactory::Instance()->Init()) {
					//I planned to use a simple file parser for this, but it would introduce complications when indexing.
					//I opted for this mostly for simplicity
					//OBS: lowercase: full font <=> UPPERCASE: outline font		
					//Texture index
					//MessageFactory::Instance()->SetColor(255, 255, 255, 255);
					TextureManager::Instance()->Add(MessageFactory::Instance()->Export("gosh  dang  to  heck !"));	//8
					TextureManager::Instance()->Add(MessageFactory::Instance()->Export("A  PROFANITY FREE  GAME"));	//9
					TextureManager::Instance()->Add(MessageFactory::Instance()->Export("play"));					//10
					TextureManager::Instance()->Add(MessageFactory::Instance()->Export("quit"));					//11
					TextureManager::Instance()->Add(MessageFactory::Instance()->Export("character select"));		//12
					TextureManager::Instance()->Add(MessageFactory::Instance()->Export("back"));					//13
					TextureManager::Instance()->Add(MessageFactory::Instance()->Export("game paused"));				//14
					TextureManager::Instance()->Add(MessageFactory::Instance()->Export("resume"));					//15
					TextureManager::Instance()->Add(MessageFactory::Instance()->Export("u  dead !"));				//16
					TextureManager::Instance()->Add(MessageFactory::Instance()->Export("retry"));					//17
					TextureManager::Instance()->Add(MessageFactory::Instance()->Export("title"));					//18
				}
				else {

					cout << "fonts not initialized" << endl;
					return false;
				}
			}
			else
			{
				cout << "Renderer init fail!" << endl;
				return false; // Renderer init fail. 
			}

			if (AudioManager::Instance()->Init()) {
				//Music and Sound Indices:
				AudioManager::Instance()->AddMusic("Sound/Chase.mp3"); //0
				AudioManager::Instance()->AddMusic("Sound/Death.mp3"); //1
				AudioManager::Instance()->AddChunk("Sound/SCD_FM_02.wav"); //0
			}
			else
			{
				cout << "Sound init fail!" << endl;
				return false; // SDL init fail. 
			}
		}
		else
		{
			cout << "Window init fail!" << endl;
			return false; // Window init fail. 
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
	AudioManager::Instance()->Release();
	MessageFactory::Instance()->Release();
	TextureManager::Instance()->Release();
	RendererManager::Instance()->Release();
	WindowManager::Instance()->Release();
	IMG_Quit();
	SDL_Quit();
}