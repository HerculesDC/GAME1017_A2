#pragma once
#include <vector>
#include "States.hpp"

/* I couldn't think of any better way to track the player, so I placed that 
 * responsibility on the State Machine, since it's the one responsible for managing 
 * cross-state functionality and information
 * Technically, it's preferrable to use enum classes and define them using scope resolution 
 * upon usage but I'm using a plain enum, for simplicity
 */
enum MachineStates { TITLE = 0, MENU, GAME, PAUSE, LOSE, QUIT };

class StateMachine {
	public:
		static StateMachine& Instance(); //changed for use with functional

		bool RequestStateChange(void* toState);

		int RequestPlayerID() { return m_iPlayer; }
		void SetPlayer(int playerIndex) { m_iPlayer = (playerIndex % 3); }

		void Update();
		void Render();
		
		void Clean();

	private:
		StateMachine();
		compl StateMachine();

		void PushState(State* state);
		void DestroyState(); //will always destroy the last one

		int m_iPlayer;
		MachineStates m_innerState;
		std::vector<State*> m_vStates;
};