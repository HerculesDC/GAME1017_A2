#include <iostream>
#include "Obstacle.hpp"
#include "Managers.hpp"

Obstacle::Obstacle(int xStart, int yStart, ObstacleType oType, int speed, int index,
				   bool isPlatform, double rotSpeed):
	m_iX(xStart), m_iY(yStart), m_iSpeed(speed)
{
	SDL_Rect tempDest;
		switch (oType) {
		case BUZZSAW:
			tempDest = { m_iX, m_iY, m_iObstacleSize, m_iObstacleSize };
			m_pSprite = new Sprite(index, tempDest, { m_iObstacleSize, m_iObstacleSize, m_iObstacleSize, m_iObstacleSize });
			m_rColl = tempDest;
			m_bRotates = true;
			if (m_bRotates) { m_dRotSpeed = rotSpeed; }
			break;
		case SPIKED_FLOOR:
			tempDest = { m_iX, m_iY, m_iObstacleSize, m_iObstacleSize/2 };
			m_pSprite = new Sprite(index, tempDest, { m_iObstacleSize, m_iObstacleSize/2, m_iObstacleSize, m_iObstacleSize/2 });
			m_rColl = tempDest;
			m_bRotates = false;
			if (m_bRotates) { m_dRotSpeed = rotSpeed; }
			else m_bRotates = 0.0;
			break;
		case SPIKED_WALL:
			tempDest = { m_iX, m_iY, m_iObstacleSize, 7* m_iObstacleSize / 2 };
			m_pSprite = new Sprite(index, tempDest, { 0, 0, m_iObstacleSize, 7 * m_iObstacleSize/2 });
			m_rColl = tempDest;
			m_bRotates = false;
			if (m_bRotates) { m_dRotSpeed = rotSpeed; }
			else m_bRotates = 0.0;
			break;
		default:
			m_pSprite = nullptr;
			m_bRotates = false;
		}
}

Obstacle::Obstacle(int xStart, int speed, bool hasSprite, int index,
				   SDL_Rect destination, SDL_Rect source,
				   bool isPlatform, bool rotates, double rotSpeed):
	m_iX(xStart), m_iSpeed(speed)
{
	//untaken parameter: isPlaform
	if (hasSprite) {
		m_pSprite = new Sprite(index, destination, source);
		m_rColl = destination;
		m_bRotates = rotates;
		if (m_bRotates) { m_dRotSpeed = rotSpeed; }
	}
	else m_pSprite = nullptr;
}

Obstacle::compl Obstacle() {
	if (m_pSprite != nullptr) {
		delete m_pSprite;
		m_pSprite = nullptr;
	}
}

void Obstacle::Update() {
	
	m_iX -= m_iSpeed;
	
	if (m_pSprite != nullptr) { 
		m_pSprite->SetX(m_iX);
		m_rColl.x = m_iX;
	}
	if (m_bRotates) m_dAngle += m_dRotSpeed;
}

void Obstacle::Render() {

	if (m_pSprite != nullptr) {
		SDL_RenderCopyEx(RendererManager::Instance()->GetRenderer(), m_pSprite->GetTexture(), 
						 m_pSprite->GetSrcP(), m_pSprite->GetDstP(), 
						 m_dAngle, NULL, SDL_FLIP_NONE);

		//obs.: NULL centers the rotation point, SDL_FLIP_NONE ensures sprite isn't flipped
		//COLLISION SPECS (will be refactored
		/*
		SDL_SetRenderDrawColor(RendererManager::Instance()->GetRenderer(), 255, 128, 128, 100);
		SDL_SetRenderDrawBlendMode(RendererManager::Instance()->GetRenderer(), SDL_BLENDMODE_ADD);
		SDL_RenderFillRect(RendererManager::Instance()->GetRenderer(), &m_rColl);
		*/
	}
}