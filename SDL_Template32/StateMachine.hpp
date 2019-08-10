#pragma once
#include <vector>
#include "States.hpp"

enum MachineStates {TITLE, MENU, GAME, PAUSE, LOSE, QUIT};

class StateMachine {
	public:
		static StateMachine& Instance(); //changed for use with functional

		bool RequestStateChange(void* toState);

		void Update();
		void Render();
		
		void Clean();

	private:
		StateMachine();
		compl StateMachine();

		void PushState(State* state);
		void DestroyState(); //will always destroy the last one

		MachineStates m_innerState;
		std::vector<State*> m_vStates;
};