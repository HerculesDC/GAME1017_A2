#include "States.hpp"
#include "Game.h"
#include "Managers.hpp"

//base class
State::State() {}
State::compl State() {}

void State::Render() { SDL_RenderPresent(Game::Instance()->GetRenderer()); }


//Inherited classes:
//	-> Title
TitleState::TitleState() {}
TitleState::compl TitleState() {}

void TitleState:: Update() {}
void TitleState::Render() {}
void TitleState::Enter() {}
void TitleState::Pause() {}
void TitleState::Resume(){}
void TitleState::Exit() {}

//Inherited classes:
//	-> Menu
MenuState::MenuState() {}
MenuState::compl MenuState() {}

void MenuState::Update() {}
void MenuState::Render() {}
void MenuState::Enter() {}
void MenuState::Pause() {}
void MenuState::Resume() {}
void MenuState::Exit() {}

//Inherited classes:
//	-> Game
GameState::GameState() {}
GameState::compl GameState() {}
void GameState::Update() {}
void GameState::Render() {}
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
void LoseState::Render() {}
void LoseState::Enter() {}
void LoseState::Pause() {}
void LoseState::Resume() {}
void LoseState::Exit() {}