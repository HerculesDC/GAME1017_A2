#include <iostream>
#include "States.hpp"
#include "Managers.hpp"
#include "Sprite.hpp"
#include "StateMachine.hpp"
#include "Obstacle.hpp"

enum SoundStates { LOOP = -1, NO_LOOP, CHUNK_CHANNEL, };

//base class
State::State() {}
State::compl State() {}

void State::Update() {

	if (!m_vSprites.empty()) {
		for (std::vector<Sprite*>::iterator it = m_vSprites.begin(); it != m_vSprites.end(); it++) {
			(*it)->Update();
		}
	}
}

void State::Render() { 

		//this makes all of the sprites(and subclasses) be rendered accordingly.
		//I'm relying on their inheritance to achieve proper behavior
		//And since this function is overridable, I don't have to worry reimplementing this sort of stuff
	if (!m_vSprites.empty()) {
		for (std::vector<Sprite*>::iterator it = m_vSprites.begin(); it != m_vSprites.end(); ++it) {
			(*it)->Render();
		}
	}
	SDL_RenderPresent(RendererManager::Instance()->GetRenderer());
}

//Inherited classes:
//	-> Title
TitleState::TitleState() {
	m_vSprites.reserve(1);
}
TitleState::compl TitleState() {}

void TitleState::Enter() {
	std::cout << "enter title" << std::endl;

	SDL_Rect* temp = nullptr;
	//6: Title Text
	temp = TextureManager::Instance()->GetSize(6, 50, 75);
	m_vSprites.push_back(new Sprite(6, *temp));
	
	//7: Subtitle Text
	temp = TextureManager::Instance()->GetSize(7, 25, 150);
	m_vSprites.push_back(new Sprite(7, *temp));

	//8: Play text
	temp = TextureManager::Instance()->GetSize(8, 300, 400);
	m_vSprites.push_back(new Sprite(8, *temp));

	//9: Quit text
	temp = TextureManager::Instance()->GetSize(9, 550, 400);
	m_vSprites.push_back(new Sprite(9, *temp));

	/*
	 * ANIMATION TEST (PASSED)
	//5:Player sprite for test
	m_vSprites.push_back(new AnimatedSprite(5, 2));
	//this is just for test for now
	m_vSprites.back()->SetDest({(1024/2)-64,(768/2)-64, 128, 128});
	m_vSprites.back()->SetState(new SpriteState(DYING));
	*/

	//manually setting buttons for now
	Command* c = new StateChangeCommand;
	temp = TextureManager::Instance()->GetSize(1, 300, 500);
	m_vSprites.push_back(new Button(*c, &Command::Execute, new MachineStates(GAME), 1)); //1: play button image
	m_vSprites.back()->SetDest(*temp);

	Command* q = new QuitCommand;
	m_vSprites.push_back(new Button(*q, &Command::Execute, new MachineStates(QUIT), 2)); //2: quit button image
	temp = TextureManager::Instance()->GetSize(2, 565, 500);
	m_vSprites.back()->SetDest(*temp);
}

void TitleState:: Update() {  //Other functionality goes here
	State::Update();
}

void TitleState::Render() {

	SDL_SetRenderDrawColor(RendererManager::Instance()->GetRenderer(), 255, 255, 255, 255);
	SDL_RenderClear(RendererManager::Instance()->GetRenderer());
	

	//I treated this one background differently. Passing null pointers to use the entire screen
	SDL_RenderCopy(RendererManager::Instance()->GetRenderer(), TextureManager::Instance()->Retrieve(0), nullptr, nullptr);
	
	State::Render();
}

void TitleState::Pause() { std::cout << "pause title" << std::endl; }
void TitleState::Resume() { std::cout << "resume title" << std::endl; }
void TitleState::Exit() { std::cout << "exit title" << std::endl;  }

//Inherited classes:
//	-> Menu
//		-> Thinking of implementing "menu" as a sort of "Pause" of "Title".
//			-> Will require rework actually on the StateMachine
MenuState::MenuState() {}
MenuState::compl MenuState() {}

void MenuState::Enter() { std::cout << "enter menu" << std::endl; }

void MenuState::Update() { State::Update(); }

void MenuState::Render() {

	SDL_SetRenderDrawColor(RendererManager::Instance()->GetRenderer(), 0, 255, 255, 255);
	SDL_RenderClear(RendererManager::Instance()->GetRenderer());

	//thinking of implementing characters as buttons that animate on mouseover...
	//may also require a button for "back." No quit planned for this screen

	State::Render();
}


void MenuState::Pause() { std::cout << "pause menu" << std::endl; }
void MenuState::Resume() { std::cout << "resume menu" << std::endl; }
void MenuState::Exit() { std::cout << "exit menu" << std::endl; }

//Inherited classes:
//	-> Game
//reversed my decision to use an enum class in this case. 
//These values are informative, and should be readily convertible
enum ObstacleInfo { OBST_THRESHOLD = 3, MAX_OBST = 9, OBST_SIZE = 128, };

GameState::GameState() : m_iNumObst(0) {
	m_vObstacles.reserve(MAX_OBST);
	m_vForegrounds.reserve(3);
}

GameState::compl GameState() {}

void GameState::Enter() {

	//OBS: Still not sure where to put the death sound

	if (!Mix_PlayingMusic()) Mix_PlayMusic(AudioManager::Instance()->GetMusic(0), LOOP);

	std::cout << "enter game" << std::endl;
	//Scenery setup:
	int tempIndex = 3; //index common to backgrounds

	//Background specs
	int tempSpeed = 2; //I halved the framerate, so I'm doubling the speeds
	SDL_Rect tempSrc  = {0, 0, 1024, 768};
	SDL_Rect tempDest = {0, 0, 1024, 768};
	
	for (int i = 0; i < 2; ++i) {
		tempDest.x = i * tempDest.w;
		m_vSprites.push_back(new Background(tempIndex, tempDest, tempSrc, tempSpeed));
	}
	
	tempSpeed = 6;
	tempSrc  = {1024, 0, 256, 512};
	tempDest = {   0, 0, 256, 512};
	
	for (int i = 0; i < 5; ++i) {
		tempDest.x = i * tempDest.w;
		m_vSprites.push_back(new Background(tempIndex, tempDest, tempSrc, tempSpeed));
	}

	tempSpeed = 6;
	tempSrc  = { 1024, 512, 512, 256};
	tempDest = {    0, 512, 512, 256};

	for (int i = 0; i < 3; ++i) {
		tempDest.x = i * tempDest.w;
		m_vForegrounds.push_back(new Background(tempIndex, tempDest, tempSrc, tempSpeed));
	}

	//because Obstacles and Player will be different kinds of objects, 
	//	they can't be initialized into the Sprite vector
	//	initialize with empty obstacles
	for (int i = 0; i < MAX_OBST; ++i) { 
		m_vObstacles.push_back(new Obstacle(i * OBST_SIZE));
	}

	std::cout << "Obstacles: " << m_vObstacles.size() << std::endl;
}

void GameState::Update() {//OBS: The State Machine handles the pause
	
	if (CommandHandler::Instance()->GetKeyDown(SDL_SCANCODE_P)) 
		StateMachine::Instance().RequestStateChange(new MachineStates(PAUSE));
	
	if (!m_vSprites.empty()) {
		for (std::vector<Sprite*>::iterator it = m_vSprites.begin(); it != m_vSprites.end(); it++) {
			(*it)->Update();
		}
	}

	if (!m_vObstacles.empty()) {

		if (m_vObstacles.front()->GetX() < -(OBST_SIZE)) {
			delete m_vObstacles.front();
			m_vObstacles.erase(m_vObstacles.begin());
			if (m_iNumObst == 0) {
				m_vObstacles.push_back(new Obstacle(8 * OBST_SIZE, 6, true, 4, 
					{8* OBST_SIZE, 448, OBST_SIZE, OBST_SIZE }, { OBST_SIZE , OBST_SIZE , OBST_SIZE , OBST_SIZE },
					false, true, 5.0));
			}
			else m_vObstacles.push_back(new Obstacle(8 * OBST_SIZE));
			
			if (m_iNumObst == OBST_THRESHOLD) { m_iNumObst = 0; }
			else ++m_iNumObst;
		}

		for (std::vector<Obstacle*>::iterator it = m_vObstacles.begin(); it != m_vObstacles.end(); it++) {
			(*it)->Update();
		}

		for (std::vector<Sprite*>::iterator it = m_vForegrounds.begin(); it != m_vForegrounds.end(); it++) {
			(*it)->Update();
		}
	}
}

void GameState::Render() { //Can't rely on State's original rendering, due to its multilayer approach

	SDL_SetRenderDrawColor(RendererManager::Instance()->GetRenderer(), 255, 255, 255, 255);
	SDL_RenderClear(RendererManager::Instance()->GetRenderer());

	if (!m_vSprites.empty()) {
		for (std::vector<Sprite*>::iterator it = m_vSprites.begin(); it != m_vSprites.end(); ++it) {
			(*it)->Render();
		}
	}

	for (std::vector<Obstacle*>::iterator it = m_vObstacles.begin(); it != m_vObstacles.end(); it++) {
		(*it)->Render();
	}

	for (std::vector<Sprite*>::iterator it = m_vForegrounds.begin(); it != m_vForegrounds.end(); it++) {
		(*it)->Render();
	}

	SDL_RenderPresent(RendererManager::Instance()->GetRenderer());
}

void GameState::Pause() { 
	std::cout << "pause game" << std::endl;
	if (Mix_PlayingMusic()) Mix_PauseMusic();
}

void GameState::Resume() { 
	std::cout << "resume game" << std::endl;
	if (Mix_PausedMusic()) Mix_ResumeMusic();
	//else if (!Mix_PlayingMusic()) Mix_PlayMusic(AudioManager::Instance()->GetMusic(0), LOOP);
}

void GameState::Exit() { 
	std::cout << "exiting game" << std::endl;
	if (Mix_PlayingMusic()) Mix_HaltMusic();
}

bool GameState::CheckCollision(SDL_Rect bound1, SDL_Rect bound2) { return false; }

//Inherited classes:
//	-> Pause
PauseState::PauseState() {}
PauseState::compl PauseState() {}

void PauseState::Enter() {

	if (Mix_PlayingMusic()) Mix_PauseMusic();

	std::cout << "enter pause" << std::endl;

	SDL_Rect* temp = nullptr;
	//12: Pause Text
	temp = TextureManager::Instance()->GetSize(12, 350, 75);
	m_vSprites.push_back(new Sprite(12, *temp));

	//13: Resume Text
	temp = TextureManager::Instance()->GetSize(13, 200, 400);
	m_vSprites.push_back(new Sprite(13, *temp));

	//1: play button image
	Command* c = new StateChangeCommand;
	m_vSprites.push_back(new Button(*c, &Command::Execute, new MachineStates(GAME), 1));
	temp = TextureManager::Instance()->GetSize(1, 200, 500);
	m_vSprites.back()->SetDest(*temp);

	//16: Title Text
	temp = TextureManager::Instance()->GetSize(16, 500, 400);
	m_vSprites.push_back(new Sprite(16, *temp));

	//1: Play button image. Will implement proper button if I have the time
	m_vSprites.push_back(new Button(*c, &Command::Execute, new MachineStates(TITLE), 1));
	temp = TextureManager::Instance()->GetSize(1, 500, 500);
	m_vSprites.back()->SetDest(*temp);

	//9: Quit Text
	temp = TextureManager::Instance()->GetSize(9, 750, 400);
	m_vSprites.push_back(new Sprite(9, *temp));

	//this begets a new command, actually...
	Command* q = new QuitCommand;
	m_vSprites.push_back(new Button(*q, &Command::Execute, new MachineStates(QUIT), 2)); //2: quit button image
	temp = TextureManager::Instance()->GetSize(2, 800, 500);
	m_vSprites.back()->SetDest(*temp);
}

void PauseState::Update() { State::Update(); }

void PauseState::Render() {
	//requires setting blend mode because it's a semitransparent overlay
	SDL_SetRenderDrawColor(RendererManager::Instance()->GetRenderer(), 127, 127, 127, 50);
	SDL_SetRenderDrawBlendMode(RendererManager::Instance()->GetRenderer(), SDL_BLENDMODE_BLEND);
	//we want to render an overlay, not clear the screen
	SDL_RenderFillRect(RendererManager::Instance()->GetRenderer(), nullptr);

	State::Render();
}

void PauseState::Pause() { std::cout << "pause pause" << std::endl; }
void PauseState::Resume() { std::cout << "resume pause" << std::endl; }

void PauseState::Exit() { 
	std::cout << "exit pause" << std::endl;
	if (Mix_PausedMusic()) Mix_ResumeMusic();
}

//Inherited classes:
//	-> Lose
//CHECK PAUSE STATE FOR REWORK
LoseState::LoseState() {}
LoseState::compl LoseState() {}

void LoseState::Enter() { std::cout << "enter lose" << std::endl; }

void LoseState::Update() {
	State::Update();
}

void LoseState::Render() {
	//requires setting blend mode because it's a semitransparent overlay
	SDL_SetRenderDrawColor(RendererManager::Instance()->GetRenderer(), 255, 0, 0, 150);
	SDL_SetRenderDrawBlendMode(RendererManager::Instance()->GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_RenderFillRect(RendererManager::Instance()->GetRenderer(), nullptr);

	State::Render();
}

void LoseState::Pause() { std::cout << "pause lose" << std::endl; }
void LoseState::Resume() { std::cout << "resume lose" << std::endl; }
void LoseState::Exit() { std::cout << "exit lose" << std::endl; }