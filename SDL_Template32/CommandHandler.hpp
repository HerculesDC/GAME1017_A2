#pragma once
#include "SDL.h"

//aggregating command and command handler, for easier management

class Command {
	public:
		Command() {}
		virtual compl Command() {}
		virtual bool Execute(void * param) = 0;
};

class StateChangeCommand : public Command{
	virtual bool Execute(void* exec) override;
};

class QuitCommand : public Command {
	virtual bool Execute(void* exec) override;
};

class KeyCommand : public Command {
	virtual bool Execute(void* exec) override;
};

class PlayerChangeCommand : public Command {
	virtual bool Execute(void* exec) override;
};

class CommandHandler {
	public:
		static CommandHandler* Instance();
		Command* IssueCommand();

		void HandleEvents();
		bool GetKeyDown(SDL_Scancode c);
		const SDL_MouseButtonEvent* GetMouseButton() const { return m_pMouseButton; }
		const SDL_MouseMotionEvent* GetMouseMotion() const { return m_pMouseMotion; }
		const SDL_Point* GetMouse() const { return const_cast<SDL_Point*>(&m_Mouse); }

	private:
		CommandHandler();
		compl CommandHandler();
		//will require redefinition for mouse states
		SDL_Point m_Mouse;
		
		const Uint8* m_piKeyStates;
		const SDL_MouseButtonEvent* m_pMouseButton;
		const SDL_MouseMotionEvent* m_pMouseMotion;
};