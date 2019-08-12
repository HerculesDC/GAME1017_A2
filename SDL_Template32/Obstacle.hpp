/* My plan is to transform the obstacles into "self-managing units" that know what
 * to do and how to create themselves based solely on their own type.
 * I plan to do this especially because the obstacles are always instantiated at the end of the
 * obstacle vector.
 * On another note, I doubled the angle (compared to the example) because the framerate was halved
 */
#pragma once
#include "Sprite.hpp"

enum class ObstacleType {EMPTY, BUZZSAW, SPIKED_FLOOR, SPIKED_WALL,};

class Obstacle {
	public:
		//Obstacle(ObstacleType oType = ObstacleType::EMPTY);
		Obstacle(int xStart, int speed = 6, bool hasSprite = false, int index = 4,
				 SDL_Rect destination = {0, 0, 0, 0}, SDL_Rect source = { 0, 0, 0, 0 },
				 bool isPlatform = false, bool rotates = false, double rotSpeed = 10.0);
		compl Obstacle();

		int GetX() const { return m_iX; }
		bool Rotates() const { return m_bRotates; }
		double GetAngle() const { return m_dAngle; }
		Sprite GetSprite() const { return *m_pSprite; }
		Sprite* GetSpriteP() const { return m_pSprite; }
		SDL_Rect GetCollRect() const { return m_rColl; }
		SDL_Rect* GetCollRectP() { return &m_rColl; }

		void Update();
		void Render();

	private:
		//no platforms planned for now
		Sprite* m_pSprite;
		SDL_Rect m_rColl; //collision rect
		double m_dAngle, m_dRotSpeed;
		int m_iX, m_iSpeed;
		bool m_bRotates;
};