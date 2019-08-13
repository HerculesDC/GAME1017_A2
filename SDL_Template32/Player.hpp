#pragma once
#include "Sprite.hpp"

class Player {
	public:
		Player();
		compl Player();

		AnimatedSprite* GetSprite() { return &m_aSprite; }
		SDL_Rect* GetColl() { return &m_rColl; }
		bool IsGrounded() const { return m_bIsGrounded; }

		void Update();
		void Render();

	private:
		AnimatedSprite m_aSprite;
		SDL_Rect m_rColl;
		double m_dAccelx, m_dAccelY,
			m_dMaxAccelX,
			m_dVelX, m_dVelY,
			m_dDrag, m_dGrav;
		bool m_bIsGrounded;
};