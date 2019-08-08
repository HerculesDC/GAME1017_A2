#pragma once


class Sprite;
class PlayButton;

class State{
	public:
		State();
		virtual compl State();
		virtual void Update() = 0;
		virtual void Render(); //NOT ABSTRACT!!!
		virtual void Enter() = 0;
		virtual void Pause() = 0;
		virtual void Resume() = 0;
		virtual void Exit() = 0;
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