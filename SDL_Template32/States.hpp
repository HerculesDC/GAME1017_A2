#pragma once
#include <vector>
#include "SDL.h"
#include "Player.hpp"

//class Sprite;
class Obstacle;
//class Player;

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

		int GetPlayer() const { return m_iPlayerIndex; }

	private:
		int m_iPlayerIndex;
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

		// !IsAlive() triggers death animation
		// IsDead() triggers state transition
		bool PlayerAlive() { return !m_pPlayer->IsDead(); }

	private:
		bool CheckCollision();

	private:	
		SDL_Rect m_rGroundRect; //data for ground collision
		std::vector<Obstacle*> m_vObstacles;
		std::vector<Sprite*> m_vForegrounds;
		Player* m_pPlayer; //may consider converting to object
		int m_iNumObst;
		bool m_bPlayedDeath;
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