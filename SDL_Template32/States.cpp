#include <iostream>
#include <cstdlib>
#include "States.hpp"
#include "Managers.hpp"
#include "Sprite.hpp"
#include "StateMachine.hpp"
#include "Obstacle.hpp"
#include "Player.hpp"

//base class
State::State() {}
State::compl State() {}

void State::Update() {

	if (!m_vSprites.empty()) {
		for (std::vector<Sprite*>::iterator it = m_vSprites.begin(); it != m_vSprites.end(); it++) {
			if ((*it) != nullptr) { 
				(*it)->Update();
			}
			else { std::cout << "empty item" << std::endl; }
		}
	}
}

void State::Render() { 

	if (!m_vSprites.empty()) {
		for (std::vector<Sprite*>::iterator it = m_vSprites.begin(); it != m_vSprites.end(); ++it) {
			if ((*it) != nullptr) (*it)->Render();
		}
	}
	SDL_RenderPresent(RendererManager::Instance()->GetRenderer());
}

//Inherited classes: Title
TitleState::TitleState() {
	m_vSprites.reserve(1);
}
TitleState::compl TitleState() {}

void TitleState::Enter() {
	std::cout << "enter title" << std::endl;

	SDL_Rect* temp = nullptr;
	//8: Title text index
	temp = TextureManager::Instance()->GetSize(8, 50, 75);
	m_vSprites.push_back(new Sprite(8, *temp));
	
	//9: Subtitle text index
	temp = TextureManager::Instance()->GetSize(9, 25, 150);
	m_vSprites.push_back(new Sprite(9, *temp));

	//10: Play text index
	temp = TextureManager::Instance()->GetSize(10, 300, 400);
	m_vSprites.push_back(new Sprite(10, *temp));

	//11: Quit text index
	temp = TextureManager::Instance()->GetSize(11, 550, 400);
	m_vSprites.push_back(new Sprite(11, *temp));
	
	//2: Play button image index
	Command* c = new StateChangeCommand;
	temp = TextureManager::Instance()->GetSize(2, 300, 500);
	m_vSprites.push_back(new Button(*c, &Command::Execute, new MachineStates(MENU), false, 2));
	m_vSprites.back()->SetDest(*temp);

	//3: Quit button image index
	Command* q = new QuitCommand;
	m_vSprites.push_back(new Button(*q, &Command::Execute, new MachineStates(QUIT), false, 3));
	temp = TextureManager::Instance()->GetSize(3, 565, 500);
	m_vSprites.back()->SetDest(*temp);

	m_vSprites.shrink_to_fit();
}

void TitleState:: Update() {  //Other functionality goes here
	State::Update();
}

void TitleState::Render() {

	SDL_SetRenderDrawColor(RendererManager::Instance()->GetRenderer(), 255, 255, 255, 255);
	SDL_RenderClear(RendererManager::Instance()->GetRenderer());
	
	//I treat backgrounds differently, because they don't require speicific placement.
	//I just pass null pointers to the source and destination, to render the whole image to the whole screen
	SDL_RenderCopy(RendererManager::Instance()->GetRenderer(), TextureManager::Instance()->Retrieve(0), nullptr, nullptr);
	
	State::Render();
}

void TitleState::Pause() { /*It shouldn't be posible to pause the title*/ }
void TitleState::Resume() { /*It shouldn't be posible to unpause the title*/ }

void TitleState::Exit() { }

//Inherited classes: Menu
MenuState::MenuState() : m_iPlayerIndex(0){}
MenuState::compl MenuState() {}

void MenuState::Enter() {
	//REWORK LAYOUT!
	std::cout << "enter menu" << std::endl;

	SDL_Rect* temp = nullptr;
	//8: Title text index
	temp = TextureManager::Instance()->GetSize(8, 50, 75);
	m_vSprites.push_back(new Sprite(8, *temp));

	//12: Character select text index
	temp = TextureManager::Instance()->GetSize(12, 100, 150);
	m_vSprites.push_back(new Sprite(12, *temp));

	Command* g = new StateChangeCommand;
	
	for (int i = 0; i < 3; ++i) {
		//7:Player sprite, animated button
		m_vSprites.push_back(new AnimatedButton(*g, &Command::Execute, new MachineStates(GAME), false, i, 7));
		m_vSprites.back()->SetDest({ 200 + (i*200), (768 / 2) - 64, 128, 128 });
	}

	//13: Back text index
	temp = TextureManager::Instance()->GetSize(13, 25, 550);
	m_vSprites.push_back(new Sprite(13, *temp));

	//4: back button image. Reuses the StateChangeCommand initialized earlier
	m_vSprites.push_back(new Button(*g, &Command::Execute, new MachineStates(TITLE), false, 4));
	temp = TextureManager::Instance()->GetSize(2, 265, 500);
	m_vSprites.back()->SetDest(*temp);

	//11: Quit text index
	temp = TextureManager::Instance()->GetSize(11, 700, 550);
	m_vSprites.push_back(new Sprite(11, *temp));

	//3: quit button image
	Command* q = new QuitCommand;
	m_vSprites.push_back(new Button(*q, &Command::Execute, new MachineStates(QUIT), false, 3)); 
	temp = TextureManager::Instance()->GetSize(2, 565, 500);
	m_vSprites.back()->SetDest(*temp);

	m_vSprites.shrink_to_fit();
}

void MenuState::Update() { 
	State::Update();
	//I know there should be a more elegant way, but honestly, I'm in it for the practice
	for (std::vector<Sprite*>::iterator it = m_vSprites.begin(); it != m_vSprites.end(); ++it) {
		
		AnimatedButton* test = dynamic_cast<AnimatedButton*>((*it));
		if (test != nullptr) {
			if (test->GetCurState() == MOUSEDOWN) {
				m_iPlayerIndex = test->GetPlayer();
			}
		}
	}
}

void MenuState::Render() {

	SDL_SetRenderDrawColor(RendererManager::Instance()->GetRenderer(), 0, 255, 255, 255);
	SDL_RenderClear(RendererManager::Instance()->GetRenderer());

	SDL_RenderCopy(RendererManager::Instance()->GetRenderer(), TextureManager::Instance()->Retrieve(1), nullptr, nullptr);

	State::Render();
}

void MenuState::Pause() { /*It shouldn't be posible to pause the menu*/ }
void MenuState::Resume() {/*It shouldn't be posible to unpause the menu, either*/ }
void MenuState::Exit() {}

//Inherited classes: Game
//These values are merely informative, and should be readily convertible
enum SpriteInfo { OBST_THRESHOLD = 3, MAX_OBST = 9, SPRITE_SIZE = 128, };

GameState::GameState() : m_iNumObst(0) {
	m_vObstacles.reserve(MAX_OBST);
	m_vForegrounds.reserve(3);
	m_rGroundRect = {0, 512, 1024, 256};
}

GameState::compl GameState() {}

void GameState::Enter() {

	m_bPlayedDeath = false;
	
	//OBS: Still not sure where to put the death sound

	if (!Mix_PlayingMusic()) Mix_PlayMusic(AudioManager::Instance()->GetMusic(0), LOOP);

	std::cout << "enter game" << std::endl;
	//Scenery setup:
	int tempIndex = 5; //index common to backgrounds

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

	for (int i = 0; i < MAX_OBST; ++i) { 
		m_vObstacles.push_back(new Obstacle(i * SPRITE_SIZE, 0, ObstacleType::EMPTY));
	}

	m_pPlayer = new Player(StateMachine::Instance().RequestPlayerID());
	m_pPlayer->GetSprite()->SetDest({1024/4, 512-256, SPRITE_SIZE, SPRITE_SIZE});
	m_pPlayer->GetSprite()->SetState(new SpriteState(JUMPING));

	m_vSprites.shrink_to_fit();
	m_vForegrounds.shrink_to_fit();
	m_vObstacles.shrink_to_fit();
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

		if (m_vObstacles.front()->GetX() < -(SPRITE_SIZE)) {
			delete m_vObstacles.front();
			m_vObstacles.erase(m_vObstacles.begin());
			if (m_iNumObst == 0) { 
				switch (rand() % 3) {
				case 0:
					m_vObstacles.push_back(new Obstacle(8 * SPRITE_SIZE, 448, ObstacleType::BUZZSAW));
					break;
				case 1:
					m_vObstacles.push_back(new Obstacle(8 * SPRITE_SIZE, 448, ObstacleType::SPIKED_FLOOR));
					break;
				case 2:
					m_vObstacles.push_back(new Obstacle(8 * SPRITE_SIZE, 0, ObstacleType::SPIKED_WALL));
				}
			}
			else m_vObstacles.push_back(new Obstacle(8 * SPRITE_SIZE, 0, ObstacleType::EMPTY));
			
			if (m_iNumObst == OBST_THRESHOLD) { m_iNumObst = 0; }
			else ++m_iNumObst;
		}

		for (std::vector<Obstacle*>::iterator it = m_vObstacles.begin(); it != m_vObstacles.end(); it++) {
			(*it)->Update();
		}
	}

	if (m_pPlayer) {
		if (m_pPlayer->IsAlive()) {
			m_pPlayer->SetX(1024/4);
			if (m_pPlayer->IsGrounded()) {
				if (m_pPlayer->GetState() == RUNNING) {
					if (CommandHandler::Instance()->GetKeyDown(SDL_SCANCODE_S))
						m_pPlayer->Set(new SpriteState(ROLLING));
					else if (CommandHandler::Instance()->GetKeyDown(SDL_SCANCODE_SPACE)) {
						m_pPlayer->SetGrounded(false);
						m_pPlayer->SetAccelY(-(3*m_pPlayer->GetGravity()));
						m_pPlayer->Set(new SpriteState(JUMPING));
						if (!Mix_Playing(-1)) Mix_PlayChannel(-1, AudioManager::Instance()->GetChunk(0), 0);
					}
				}
				else if (m_pPlayer->GetState() == ROLLING) {
					if (!CommandHandler::Instance()->GetKeyDown(SDL_SCANCODE_S))
						m_pPlayer->Set(new SpriteState(RUNNING));
				}
			}
		}

		if (CheckCollision()) { 
			if (Mix_PlayingMusic()) Mix_HaltMusic();
			if (Mix_Playing(-1) && !m_bPlayedDeath) { 
				Mix_HaltChannel(-1);
				Mix_PlayChannel(-1, AudioManager::Instance()->GetChunk(1), 0);
				m_bPlayedDeath = true;
			}

			m_pPlayer->Set(new SpriteState(DYING));
		}
		
		m_pPlayer->Update();
		m_pPlayer->SetAccelY(0.0);

		if (SDL_HasIntersection(m_pPlayer->GetSprite()->GetDstP(), &m_rGroundRect)) {
			m_pPlayer->SetGrounded(true);
			m_pPlayer->SetY(m_rGroundRect.y - (1+m_pPlayer->GetH()));
			if (m_pPlayer->GetState() == JUMPING) {
				m_pPlayer->Set(new SpriteState(RUNNING));
			}
		}
		else m_pPlayer->SetGrounded(false);
	}

	if (!m_vForegrounds.empty()) {

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
	if (!m_vObstacles.empty()) {
		for (std::vector<Obstacle*>::iterator it = m_vObstacles.begin(); it != m_vObstacles.end(); it++) {
			(*it)->Render();
		}
	}

	if (m_pPlayer) m_pPlayer->Render();

	if (!m_vForegrounds.empty()) {
		for (std::vector<Sprite*>::iterator it = m_vForegrounds.begin(); it != m_vForegrounds.end(); it++) {
			(*it)->Render();
		}
	}

	/*
	SDL_SetRenderDrawColor(RendererManager::Instance()->GetRenderer(), 0, 255, 0, 50);
	SDL_SetRenderDrawBlendMode(RendererManager::Instance()->GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_RenderFillRect(RendererManager::Instance()->GetRenderer(), &m_rGroundRect);
	*/

	SDL_RenderPresent(RendererManager::Instance()->GetRenderer());
}

void GameState::Pause() { 
	std::cout << "pause game" << std::endl;
	if (Mix_PlayingMusic()) Mix_PauseMusic();
}

void GameState::Resume() { 
	std::cout << "resume game" << std::endl;
	if (Mix_PausedMusic()) Mix_ResumeMusic();
	else if (!Mix_PlayingMusic()) Mix_PlayMusic(AudioManager::Instance()->GetMusic(0), LOOP);
}

void GameState::Exit() { 
	std::cout << "exiting game" << std::endl;
	if (Mix_PlayingMusic()) Mix_HaltMusic();
}

bool GameState::CheckCollision() { 

	if (!m_vObstacles.empty()) {
		for (std::vector<Obstacle*>::iterator it = m_vObstacles.begin(); it != m_vObstacles.end(); ++it) {
			if ((*it)->GetSpriteP() != nullptr) {
				if (SDL_HasIntersection(m_pPlayer->GetCollP(), (*it)->GetCollRectP()))
					return true;
			}
		}
	}
	return false;
}

//Inherited classes: Pause
PauseState::PauseState() {}
PauseState::compl PauseState() {}

void PauseState::Enter() {

	if (Mix_PlayingMusic()) Mix_PauseMusic();

	std::cout << "enter pause" << std::endl;

	SDL_Rect* temp = nullptr;

	//14: Game paused text index
	temp = TextureManager::Instance()->GetSize(14, 200, 150);
	m_vSprites.push_back(new Sprite(14, *temp));

	//15: Resume text index
	temp = TextureManager::Instance()->GetSize(15, 200, 400);
	m_vSprites.push_back(new Sprite(15, *temp));

	//18: title text index
	temp = TextureManager::Instance()->GetSize(18, 550, 400);
	m_vSprites.push_back(new Sprite(18, *temp));

	//2: Play button image index
	Command* c = new StateChangeCommand;
	temp = TextureManager::Instance()->GetSize(2, 300, 500);
	m_vSprites.push_back(new Button(*c, &Command::Execute, new MachineStates(GAME), false, 2));
	m_vSprites.back()->SetDest(*temp);

	//4: Title button image index
	m_vSprites.push_back(new Button(*c, &Command::Execute, new MachineStates(TITLE), false, 4));
	temp = TextureManager::Instance()->GetSize(4, 565, 500);
	m_vSprites.back()->SetDest(*temp);

	m_vSprites.shrink_to_fit();
}

void PauseState::Update() { State::Update(); }

void PauseState::Render() {
	//requires setting blend mode because it's a semitransparent overlay
	SDL_SetRenderDrawColor(RendererManager::Instance()->GetRenderer(), 127, 127, 127, 100);
	SDL_SetRenderDrawBlendMode(RendererManager::Instance()->GetRenderer(), SDL_BLENDMODE_BLEND);
	//we want to render an overlay, not clear the screen
	SDL_RenderFillRect(RendererManager::Instance()->GetRenderer(), nullptr);

	State::Render();
}

void PauseState::Pause() { }
void PauseState::Resume() { }

void PauseState::Exit() { 
	
	if (Mix_PausedMusic()) Mix_ResumeMusic();
	if (Mix_Playing(-1)) Mix_HaltChannel(-1);
}

//Inherited classes: Lose
LoseState::LoseState() {}
LoseState::compl LoseState() {}

void LoseState::Enter() { 
	std::cout << "enter lose" << std::endl;

	if (Mix_PlayingMusic()) Mix_HaltMusic();

	SDL_Rect* temp = nullptr;

	//16: You lose text index
	temp = TextureManager::Instance()->GetSize(16, 350, 150);
	m_vSprites.push_back(new Sprite(16, *temp));

	//17: Retry text index
	temp = TextureManager::Instance()->GetSize(17, 150, 400);
	m_vSprites.push_back(new Sprite(17, *temp));

	//18: Title text index
	temp = TextureManager::Instance()->GetSize(18, 425, 400);
	m_vSprites.push_back(new Sprite(18, *temp));

	//11: Quit text index
	temp = TextureManager::Instance()->GetSize(11, 700, 400);
	m_vSprites.push_back(new Sprite(11, *temp));

	//2: Play button image index
	Command* c = new StateChangeCommand;
	temp = TextureManager::Instance()->GetSize(2, 200, 500);
	m_vSprites.push_back(new Button(*c, &Command::Execute, new MachineStates(GAME), false, 2));
	m_vSprites.back()->SetDest(*temp);

	//4: Title button image index
	m_vSprites.push_back(new Button(*c, &Command::Execute, new MachineStates(TITLE), false, 4));
	temp = TextureManager::Instance()->GetSize(4, 475, 500);
	m_vSprites.back()->SetDest(*temp);

	//3: Quit button image index
	Command* q = new QuitCommand;
	m_vSprites.push_back(new Button(*q, &Command::Execute, new MachineStates(QUIT), false, 3));
	temp = TextureManager::Instance()->GetSize(4, 750, 500);
	m_vSprites.back()->SetDest(*temp);

	m_vSprites.shrink_to_fit();
}

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

void LoseState::Pause() { /*No reason to pause lose*/ }
void LoseState::Resume() { /*No reason to resume lose*/ }
void LoseState::Exit() { /*Cleanup comes here*/ }