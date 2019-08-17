/*
 *	Too late I found out I did things in reverse. 
 *	Sprite should be updated in regards to collision, not the other way around...
 */
#pragma once
#include "Sprite.hpp"

enum Facing { LEFT = -1, RIGHT = 1 };

class Player {
	public:
		Player(int index = 0); //GameState will signal to the player what character they are
		compl Player() = default;

		AnimatedSprite* GetSprite() { return &m_aSprite; }
		SDL_Rect* GetCollP() { return &m_rColl; }
		SpriteState GetState() const { return m_aSprite.GetCurState(); }
		bool IsGrounded() const { return m_bIsGrounded; }
		bool IsAlive() const { return m_aSprite.GetCurState() != DYING; }
		bool IsDead() const { return m_aSprite.IsDead(); }
		double GetVelX() const { return m_dVelX; }
		double GetVelY() const { return m_dVelY; }
		Facing GetFacing() const { return m_fDirection; }
		double GetGravity() const { return m_dGrav; }

		//still not sure how to implement these ones
		// This is part of the mistake, but I'm using it for simplicity
		int GetX() const { return m_aSprite.GetDst().x; }
		int GetY() const { return m_aSprite.GetDst().y; }
		int GetW() const { return m_aSprite.GetDst().w; }
		int GetH() const { return m_aSprite.GetDst().h; }

		bool Set(void* toState);

		void SetFacing(Facing f) { m_fDirection = f; }
		void SetAccelX(double aX) { m_dAccelX = aX; }
		void SetAccelY(double aY) { m_dAccelY = aY; }
		void SetGravity(double grav) { m_dGrav = grav; }
		void SetGrounded(bool g) { m_bIsGrounded = g; }
		void SetVelX(double vX) { m_dVelX = vX; }
		void SetVelY(double vY) { m_dVelY = vY; }
		void Stop() { SetVelX(0.0); SetVelY(0.0); }

		//these two require a less naive implementation, since I'm using  composition
		void SetX(int x) { m_aSprite.SetX(x); }
		void SetY(int y) { m_aSprite.SetY(y); }

		void Update();
		void Render();

	private:
		void UpdateCollision();

		//may not even be needed
		void Jump() {}
		void Roll() {}
		void Run() {}
		void Kill() {}

	private:
		AnimatedSprite m_aSprite;
		SDL_Rect m_rColl;
		double m_dAccelX, m_dAccelY,
			m_dMaxAccelX,
			m_dVelX, m_dVelY,
			m_dMaxVelX, m_dMaxVelY,
			m_dDrag, m_dGrav;
		Facing m_fDirection;
		bool m_bIsGrounded;
};