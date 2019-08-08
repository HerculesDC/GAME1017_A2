#include "States.hpp"
#include "Game.h"


//base class
State::State() {}
State::compl State() {}

void State::Render() { SDL_RenderPresent(Game::Instance()->GetRenderer()); }


//Inherited classes:
//	-> Title
void TitleState:: Update() {}
void TitleState::Render() {}
void TitleState::Enter() {}
void TitleState::Pause() {}
void TitleState::Resume(){}
void TitleState::Exit() {}

//Inherited classes:
//	-> Menu
void MenuState::Update() {}
void MenuState::Render() {}
void MenuState::Enter() {}
void MenuState::Pause() {}
void MenuState::Resume() {}
void MenuState::Exit() {}

//Inherited classes:
//	-> Game
void GameState::Update() {}
void GameState::Render() {}
void GameState::Enter() {}
void GameState::Pause() {}
void GameState::Resume() {}
void GameState::Exit() {}

//Inherited classes:
//	-> Lose
void LoseState::Update() {}
void LoseState::Render() {}
void LoseState::Enter() {}
void LoseState::Pause() {}
void LoseState::Resume() {}
void LoseState::Exit() {}