#include "CommandHandler.hpp"
#include "StateMachine.hpp"
#include "Game.h"

bool StateChangeCommand::Execute(void* exec) { return StateMachine::Instance().RequestStateChange(exec); }

bool QuitCommand::Execute(void* exec) { Game::Instance()->Quit(); return false; }

bool KeyCommand::Execute(void* exec) { //looks a bit ugly, though...
	return CommandHandler::Instance()->GetKeyDown(*(static_cast<SDL_Scancode*>(exec)));
}

CommandHandler* CommandHandler::Instance() {

	static CommandHandler* instance = new CommandHandler();
	return instance;
}

Command* CommandHandler::IssueCommand() {
	return nullptr;
}

CommandHandler::CommandHandler() : m_piKeyStates(SDL_GetKeyboardState(nullptr)), 
								   m_Mouse({ 0,0 }), m_pMouseButton(nullptr),
								   m_pMouseMotion(nullptr)
{}

CommandHandler::compl CommandHandler() {
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

void CommandHandler::HandleEvents() {
		
	SDL_Event evt;
	if (SDL_PollEvent(&evt))
	{
		switch (evt.type)
		{
		case SDL_QUIT:
			Game::Instance()->Quit(); //rig this better
			break;
		case SDL_KEYDOWN:
			break;
		case SDL_KEYUP:
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_pMouseButton = new SDL_MouseButtonEvent(evt.button);
			SDL_GetMouseState(&m_Mouse.x, &m_Mouse.y);
			break;
		case SDL_MOUSEBUTTONUP:
			m_pMouseButton = new SDL_MouseButtonEvent(evt.button);
			SDL_GetMouseState(&m_Mouse.x, &m_Mouse.y);
			break;
		case SDL_MOUSEMOTION: //verify
			m_pMouseMotion = new SDL_MouseMotionEvent(evt.motion);
			SDL_GetMouseState(&m_Mouse.x, &m_Mouse.y);
			break;
		}
	}
}