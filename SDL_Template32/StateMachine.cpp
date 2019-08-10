#include <typeinfo>
#include "StateMachine.hpp"
#include "Sprite.hpp"

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
	//I'm implementing a couple checks here
	//the idea was to also check whether the system is requiring that a same state be pushed
	//dereferencing for comparison

	MachineStates* placeHolder = static_cast<MachineStates*>(toState);

	if (!m_vStates.empty()) {

		if (*(MachineStates*)toState == m_innerState) return false; //will not change to same state

		switch (*(MachineStates*)toState) {
		case TITLE:
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
				m_innerState = GAME;
				PushState(new GameState);
			}
			else { 
				if (typeid(*(m_vStates.back())) == typeid(GameState)) m_vStates.back()->Resume();
			}
			break;
		case PAUSE:
			m_innerState = PAUSE;
			if (typeid(*(m_vStates.back())) == typeid(GameState)) PushState(new PauseState);
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
	}
	else { 
		PushState(new TitleState);
		return true;
	}
}

void StateMachine::Update() {
	if (!m_vStates.empty()) m_vStates.back()->Update();
}

void StateMachine::Render() {
	if (!m_vStates.empty()) {
		//this is me, just trying to fancify the code...
		//I'm also assuming that if there's a pause, there's necessarily something underneath
		std::vector<State*>::reverse_iterator it = m_vStates.rbegin();

		if (typeid(*(*it)) == typeid(PauseState)) {
			//++it;
			(*++it)->Render();
			--it;
		}
		(*it)->Render();
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