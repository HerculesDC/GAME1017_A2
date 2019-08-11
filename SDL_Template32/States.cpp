#include "States.hpp"
#include "Game.h"
#include "Managers.hpp"
#include "Sprite.hpp"
#include "StateMachine.hpp"

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
	
	// Now render the backgrounds. Passing null pointers to use the entire screen
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
//		-> Thinking of implementing "menu" as a sort of "Pause" of "Title".
//			-> Will require rework actually on the StateMachine
MenuState::MenuState() {}
MenuState::compl MenuState() {}

void MenuState::Update() {}

void MenuState::Render() {

	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0, 255, 255, 255);
	SDL_RenderClear(Game::Instance()->GetRenderer());

	//thinking of implementing characters as buttons that animate on mouseover...
	//may also require a button for "back." No quit planned for this screen

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

void GameState::Update() {//OBS: The State Machine handles the pause
	if (CommandHandler::Instance()->GetKeyDown(SDL_SCANCODE_SPACE)) StateMachine::Instance().RequestStateChange(new MachineStates(PAUSE));
}

void GameState::Enter() {

}

void GameState::Render() {
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0, 0, 255, 255);
	SDL_RenderClear(Game::Instance()->GetRenderer());

	State::Render();
}

void GameState::Pause() {
	
}

void GameState::Resume() {}
void GameState::Exit() {}

bool GameState::CheckCollision(SDL_Rect bound1, SDL_Rect bound2) { return false; }

//Inherited classes:
//	-> Pause
PauseState::PauseState() {}
PauseState::compl PauseState() {}

void PauseState::Enter() {
	//may be the case to mix buttons and 
	//manually setting buttons for now
	//include stuff on sprite constructors to enable taking the temp as a parameter already, 
	//instead of setting the destination separately. In all cases, it can be set again later
	//maybe prepare the buttons on Enter

	Command* c = new StateChangeCommand;
	m_vButtons.push_back(new Button(*c, &Command::Execute, new MachineStates(GAME), 1)); //1: play button image

	SDL_Rect* temp = TextureManager::Instance()->GetSize(1, 300, 500);
	m_vButtons.back()->SetDest(*temp);

	//this begets a new command, actually...
	Command* q = new QuitCommand;
	m_vButtons.push_back(new Button(*q, &Command::Execute, new MachineStates(QUIT), 2)); //2: quit button image
	temp = TextureManager::Instance()->GetSize(2, 565, 500);
	m_vButtons.back()->SetDest(*temp);

	m_vButtons.push_back(new Button(*c, &Command::Execute, new MachineStates(TITLE), 2)); //2: quit button image
	temp = TextureManager::Instance()->GetSize(2, 565, 500);
	m_vButtons.back()->SetDest(*temp);
}

void PauseState::Update() {}

void PauseState::Render() {
	//requires setting blend mode because it's a semitransparent overlay
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 255, 0, 0, 50);
	SDL_SetRenderDrawBlendMode(Game::Instance()->GetRenderer(), SDL_BLENDMODE_BLEND);
	//we want to render an overlay, not clear the screen
	SDL_RenderFillRect(Game::Instance()->GetRenderer(), nullptr);

	//something tells me that declaring these variables here causes the flickering
	SDL_RenderCopy(Game::Instance()->GetRenderer(), TextureManager::Instance()->Retrieve(11), nullptr, TextureManager::Instance()->GetSize(11, 50, 75));
	
	//FOR THE BUTTONS. Texts are "Resume"(13), "Main Menu"(16), and "Quit"(9)
	//REPOSITION!!!
	SDL_RenderCopy(Game::Instance()->GetRenderer(), TextureManager::Instance()->Retrieve(13), nullptr, TextureManager::Instance()->GetSize(13, 25, 150));
	SDL_RenderCopy(Game::Instance()->GetRenderer(), TextureManager::Instance()->Retrieve(16), nullptr, TextureManager::Instance()->GetSize(16, 300, 400));
	SDL_RenderCopy(Game::Instance()->GetRenderer(), TextureManager::Instance()->Retrieve(9), nullptr, TextureManager::Instance()->GetSize(9, 550, 400));

	State::Render();
}


void PauseState::Pause() {}
void PauseState::Resume() {}
void PauseState::Exit() {}

//Inherited classes:
//	-> Lose
//CHECK PAUSE STATE FOR REWORK
LoseState::LoseState() {}
LoseState::compl LoseState() {}

void LoseState::Enter() {}

void LoseState::Update() {}

void LoseState::Render() {
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 127, 50, 50, 255);
	SDL_RenderClear(Game::Instance()->GetRenderer());

	// Lose Text
	SDL_Rect* temp = TextureManager::Instance()->GetSize(11, 50, 75);
	SDL_RenderCopy(Game::Instance()->GetRenderer(), TextureManager::Instance()->Retrieve(11), nullptr, temp);


	//FOR THE BUTTONS: "retry"(15), "title"(16), and "quit"(9)
	//REPOSITION!!! => OBS.: May benefit from the pause menu implementation for its layout...
	temp = TextureManager::Instance()->GetSize(15, 25, 150);
	SDL_RenderCopy(Game::Instance()->GetRenderer(), TextureManager::Instance()->Retrieve(15), nullptr, temp);

	temp = TextureManager::Instance()->GetSize(16, 300, 400);
	SDL_RenderCopy(Game::Instance()->GetRenderer(), TextureManager::Instance()->Retrieve(16), nullptr, temp);

	temp = TextureManager::Instance()->GetSize(9, 550, 400);
	SDL_RenderCopy(Game::Instance()->GetRenderer(), TextureManager::Instance()->Retrieve(9), nullptr, temp);

	State::Render();
}


void LoseState::Pause() {}
void LoseState::Resume() {}
void LoseState::Exit() {}