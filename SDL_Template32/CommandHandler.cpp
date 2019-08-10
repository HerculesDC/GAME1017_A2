#include "CommandHandler.hpp"

CommandHandler* CommandHandler::Instance() {

	static CommandHandler* instance = new CommandHandler();
	return instance;
}

Command* CommandHandler::IssueCommand() {
	return nullptr;
}

CommandHandler::CommandHandler() : m_pMouse(nullptr), m_piKeyStates(nullptr) {}

CommandHandler::compl CommandHandler() {
	m_pMouse = nullptr;
	m_piKeyStates = SDL_GetKeyboardState(nullptr);
}

bool CommandHandler::GetKeyDown(SDL_Scancode c)
{
	if (m_piKeyStates != nullptr)
	{
		if (m_piKeyStates[c] == 1)
			return true;
		else
			return false;
	}
	return false;
}

const SDL_MouseButtonEvent* CommandHandler::GetMouse() {
	if (m_pMouse) return m_pMouse;
	else return nullptr;
}

#include "Game.h"
void CommandHandler::HandleEvents() {
	
	SDL_Event evt;
	if (SDL_PollEvent(&evt))
	{
		switch (evt.type)
		{
		case SDL_QUIT:
			Game::Instance()->Quit(); //rig this better
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			m_pMouse = new SDL_MouseButtonEvent(evt.button);
			break;
		}
	}
}