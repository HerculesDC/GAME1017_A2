#pragma once
#include <vector>
#include "States.hpp"

class StateMachine {
	public:
		static StateMachine* Instance();

		bool RequestStateChange(State* toState);

		void Update();
		void Render();
		
		void Clean();

	private:
		StateMachine();
		compl StateMachine();

		void PushState(State* state);
		void DestroyState(); //will always destroy the last one

		std::vector<State*> m_vStates;
};