#include <iostream>
#include "Obstacle.hpp"
#include "Game.h"

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
		SDL_RenderCopyEx(Game::Instance()->GetRenderer(), m_pSprite->GetTexture(), 
						 m_pSprite->GetSrcP(), m_pSprite->GetDstP(), 
						 m_dAngle, NULL, SDL_FLIP_NONE);
		//obs.: NULL centers the rotation point, SDL_FLIP_NONE ensures sprite isn't flipped
		//COLLISION SPECS (will be refactored
		SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 127, 127, 127, 100);
		SDL_SetRenderDrawBlendMode(Game::Instance()->GetRenderer(), SDL_BLENDMODE_ADD);
		SDL_RenderFillRect(Game::Instance()->GetRenderer(), &m_rColl);
	}
}