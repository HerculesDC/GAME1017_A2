#include <iostream>
#include <typeinfo>
#include "StateMachine.hpp"
//#include "Sprite.hpp"

StateMachine& StateMachine::Instance() {

	static StateMachine* instance = new StateMachine();
	return *instance;
}

StateMachine::StateMachine() {
	m_vStates.reserve(1);
	m_innerState = TITLE;
	PushState(new TitleState);
}

StateMachine::compl StateMachine() {
	Clean();
}

bool StateMachine::RequestStateChange(void* toState) {

	//dangerous, I know, but it's the only way I found
	MachineStates* placeholder = static_cast<MachineStates*>(toState);

	if (!m_vStates.empty()) {

		if (*(MachineStates*)toState == m_innerState) return false; //will not change to same state

		switch (*placeholder) {
		case TITLE:
			Clean(); //requires cleaning because pause-to-title stacks states
			m_innerState = TITLE;
			PushState(new TitleState);
			break;
		case MENU:
			DestroyState();
			m_innerState = MENU;
			PushState(new MenuState);
			break;
		case GAME:
			DestroyState();
			if (m_vStates.empty()) {
				PushState(new GameState);
				m_innerState = GAME;
			}
			else if (typeid(*(m_vStates.back())) == typeid(GameState)) {
				m_vStates.back()->Resume();
			}
			break;
		case PAUSE:
			m_innerState = PAUSE;
			if (typeid(*(m_vStates.back())) == typeid(GameState)) { 
				m_vStates.back()->Pause();
				PushState(new PauseState);
			}
			break;
		case LOSE:
			DestroyState();
			m_innerState = LOSE;
			PushState(new LoseState);
			break;
		case QUIT:
			m_innerState = QUIT;
			Clean();
			break;
		default:
			return false;
		}
		std::cout << m_vStates.size() << std::endl;
	}
	else { 
		PushState(new TitleState);
		return true;
	}
}

void StateMachine::Update() { if (!m_vStates.empty()) m_vStates.back()->Update(); }

void StateMachine::Render() {
	//the whole function operates on the assumption that, 
	//if the state machine isn't empty, render all states in order
	if (!m_vStates.empty()) {
		m_vStates.shrink_to_fit();
		for (int i = 0; i < m_vStates.size(); ++i) m_vStates[i]->Render();
	}
}

void StateMachine::DestroyState() {
	if (!m_vStates.empty()) {
		m_vStates.back()->Exit();
		m_vStates.back() = nullptr;
		delete m_vStates.back();
		m_vStates.pop_back();
		m_vStates.shrink_to_fit();
	}
}

void StateMachine::PushState(State* state) {

	state->Enter();
	m_vStates.push_back(state);
}

void StateMachine::Clean() {
	while (!m_vStates.empty()) {
		DestroyState();
	}
}