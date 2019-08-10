#pragma once
#include "SDL.h"

//aggregating command and command handler, for easier management

class Command {
	public:
		Command() {}
		virtual compl Command() {}
		virtual bool Execute(void * param) = 0;
};

class StateChangeCommand:public Command{
	//virtual bool Execute() override
};

class CommandHandler {
	public:
		static CommandHandler* Instance();
		Command* IssueCommand();

		void HandleEvents();
		bool GetKeyDown(SDL_Scancode c);
		const SDL_MouseButtonEvent* GetMouse();

	private:
		CommandHandler();
		compl CommandHandler();

		const Uint8* m_piKeyStates;
		const SDL_MouseButtonEvent* m_pMouse;
};