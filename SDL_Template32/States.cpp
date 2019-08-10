#include "States.hpp"
#include "Game.h"
#include "Managers.hpp"
#include "Sprite.hpp"
#include "StateMachine.hpp"
//#include "CommandHandler.hpp"

//base class
State::State() {}
State::compl State() {}

void State::Render() { SDL_RenderPresent(Game::Instance()->GetRenderer()); }

//Inherited classes:
//	-> Title
TitleState::TitleState() {
	m_vButtons.reserve(1);
}
TitleState::compl TitleState() {}

//requires the use of the <functional> header... check
void TitleState::Enter() {
	//manually setting buttons for now
	Command* c = new StateChangeCommand;
	m_vButtons.push_back(new Button(*c, &Command::Execute, new MachineStates(GAME), 1)); //1: play button image

	SDL_Rect* temp = TextureManager::Instance()->GetSize(1, 300, 500);
	m_vButtons.back()->SetDest(*temp);

	//this begets a new command, actually...
	Command* q = new QuitCommand;
	m_vButtons.push_back(new Button(*q, &Command::Execute, new MachineStates(QUIT), 2)); //2: quit button image
	temp = TextureManager::Instance()->GetSize(2, 565, 500);
	m_vButtons.back()->SetDest(*temp);
}


void TitleState:: Update() {

	for (std::vector<Button*>::iterator it = m_vButtons.begin(); it != m_vButtons.end(); it++) (*it)->Update();
}

void TitleState::Render() {

	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 255, 255, 255, 255);
	SDL_RenderClear(Game::Instance()->GetRenderer());
	
	// Now render the backgrounds. passing null pointers to use the entire screen
	SDL_RenderCopy(Game::Instance()->GetRenderer(), TextureManager::Instance()->Retrieve(0), nullptr, nullptr);

	// Texts
	SDL_Rect * temp = TextureManager::Instance()->GetSize(6, 50, 75);
	SDL_RenderCopy(Game::Instance()->GetRenderer(), TextureManager::Instance()->Retrieve(6), nullptr, temp);
	
	temp = TextureManager::Instance()->GetSize(7, 25, 150);
	SDL_RenderCopy(Game::Instance()->GetRenderer(), TextureManager::Instance()->Retrieve(7), nullptr, temp);

	temp = TextureManager::Instance()->GetSize(8, 300, 400);
	SDL_RenderCopy(Game::Instance()->GetRenderer(), TextureManager::Instance()->Retrieve(8), nullptr, temp);

	temp = TextureManager::Instance()->GetSize(9, 550, 400);
	SDL_RenderCopy(Game::Instance()->GetRenderer(), TextureManager::Instance()->Retrieve(9), nullptr, temp);

	//Button
	for (std::vector<Button*>::iterator it = m_vButtons.begin(); it != m_vButtons.end(); it++) (*it)->Render();

	State::Render();
}

void TitleState::Pause() {}
void TitleState::Resume(){}
void TitleState::Exit() {}

//Inherited classes:
//	-> Menu
MenuState::MenuState() {}
MenuState::compl MenuState() {}

void MenuState::Update() {}

void MenuState::Render() {

	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0, 0, 255, 255);
	SDL_RenderClear(Game::Instance()->GetRenderer());

	State::Render();
}

void MenuState::Enter() {}
void MenuState::Pause() {}
void MenuState::Resume() {}
void MenuState::Exit() {}

//Inherited classes:
//	-> Game
GameState::GameState() {}
GameState::compl GameState() {}
void GameState::Update() {}

void GameState::Render() {
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0, 0, 255, 255);
	SDL_RenderClear(Game::Instance()->GetRenderer());

	State::Render();
}

void GameState::Enter() {}
void GameState::Pause() {}
void GameState::Resume() {}
void GameState::Exit() {}

bool GameState::CheckCollision(SDL_Rect bound1, SDL_Rect bound2) { return false; }

//Inherited classes:
//	-> Pause
PauseState::PauseState() {}
PauseState::compl PauseState() {}
void PauseState::Update() {}
void PauseState::Render() {}
void PauseState::Enter() {}
void PauseState::Pause() {}
void PauseState::Resume() {}
void PauseState::Exit() {}

//Inherited classes:
//	-> Lose
LoseState::LoseState() {}
LoseState::compl LoseState() {}
void LoseState::Update() {}

void LoseState::Render() {

}

void LoseState::Enter() {}
void LoseState::Pause() {}
void LoseState::Resume() {}
void LoseState::Exit() {}