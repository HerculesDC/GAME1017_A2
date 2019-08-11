#pragma once
#include "Sprite.hpp"

class Obstacle {
	public:
		Obstacle();
		compl Obstacle();

		int GetX() const { return m_iX; }
		bool Rotates() const { return m_bRotates; }
		double GetAngle() const { return m_dAngle; }
		Sprite* GetSprite() const { return m_pSprite; }

	private:
		// Verify: the fact that this has a pointer 
		//to an abstract class may require rework
		Sprite* m_pSprite;
		double m_dAngle;
		int m_iX;
		bool m_bRotates;
		//will also likely require a SDL_Rect specifically for collision
};