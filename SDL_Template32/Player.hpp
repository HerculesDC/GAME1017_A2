#pragma once
#include "Sprite.hpp"

class Player {
	public:
		Player(int index = 0); //GameState will signal to the player what character they are
		compl Player() = default;

		AnimatedSprite* GetSprite() { return &m_aSprite; }
		SDL_Rect* GetCollP() { return &m_rColl; }
		bool IsGrounded() const { return m_bIsGrounded && (m_dVelY == 0); }
		bool IsAlive() const { return m_aSprite.GetCurState() != DYING; }
		double GetVelX() const { return m_dVelX; }
		double GetVelY() const { return m_dVelY; }
		
		//still not sure how to implement these ones
		int GetX() {}
		int GetY() {}


		void SetAccelX(double aX) {}
		void SetAccelY(double aY) {}
		void SetGravity(double grav) { m_dGrav = grav; }
		void SetGrounded(bool g) { m_bIsGrounded = g; }
		void SetVelX(double vX) {}
		void SetVelY(double vY) {}
		void Stop() { SetVelX(0.0); SetVelY(0.0); }

		//these two require a less naive implementation, since I'm using  composition
		void SetX(int x);// { m_rDst.x = x; }
		void SetY(int y);// { m_rDst.y = y; }
		
		void MoveX() { m_dAccelX += 0.1; }
		void Update();
		void Render();

		void Kill();

	private:
		void UpdateCollision();

	private:
		AnimatedSprite m_aSprite;
		SDL_Rect m_rColl;
		double m_dAccelX, m_dAccelY,
			m_dMaxAccelX,
			m_dVelX, m_dVelY,
			m_dMaxVelX, m_dMaxVelY,
			m_dDrag, m_dGrav;
		bool m_bIsGrounded;
};