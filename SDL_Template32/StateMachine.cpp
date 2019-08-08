#include <typeinfo>
#include "StateMachine.hpp"

StateMachine* StateMachine::Instance() {

	static StateMachine* instance = new StateMachine();
	return instance;
}

StateMachine::StateMachine() {
	m_vStates.reserve(1);
}

StateMachine::compl StateMachine() {
	Clean();
}

bool StateMachine::RequestStateChange(State* state) {
	//I'm implementing a couple checks here
	//the idea was to also check whether the system is requiring that a same state be pushed
	if (!m_vStates.empty()) {
		if(typeid(*(m_vStates.back())) == typeid(GameState) 
			&& typeid(*(state)) == typeid(PauseState)) {
			PushState(state);
		}
		else {
			DestroyState();
			PushState(state);
		}
		return true;
	}
	else {
		PushState(state); return true;
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