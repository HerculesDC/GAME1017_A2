#pragma once
#include <vector>
#include "SDL.h"
class Sprite;
class Button;

class State{
	public:
		State();
		virtual compl State();
		virtual void Update(); //NOT ABSTRACT
		virtual void Render(); //NOT ABSTRACT!!!
		virtual void Enter() = 0;
		virtual void Pause() = 0;
		virtual void Resume() = 0;
		virtual void Exit() = 0;

	protected:
		std::vector<Sprite*> m_vSprites;
};

class TitleState : public State {
	public:
		TitleState();
		compl TitleState();

		void Update() final override;
		void Render() final override;
		void Enter() final override;
		void Pause() final override;
		void Resume() final override;
		void Exit() final override;
};

class MenuState : public State {
	public:
		MenuState();
		compl MenuState();
	
		void Update() final override;
		void Render() final override;
		void Enter() final override;
		void Pause() final override;
		void Resume() final override;
		void Exit() final override;
};

class GameState : public State {
	public:
		GameState();
		compl GameState();
	
		void Update() final override;
		void Render() final override;
		void Enter() final override;
		void Pause() final override;
		void Resume() final override;
		void Exit() final override;

	private:
		//may require rework
		bool CheckCollision(SDL_Rect bb1, SDL_Rect bb2);
};

class PauseState : public State {
	public:
		PauseState();
		compl PauseState();
	
		void Update() final override;
		void Render() final override;
		void Enter() final override;
		void Pause() final override;
		void Resume() final override;
		void Exit() final override;
};

class LoseState : public State {
public:
	LoseState();
	compl LoseState();

	void Update() final override;
	void Render() final override;
	void Enter() final override;
	void Pause() final override;
	void Resume() final override;
	void Exit() final override;
};