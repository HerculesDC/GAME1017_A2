#include "Player.hpp"
#include "Managers.hpp"

Player::Player(int index) : m_aSprite(index), m_bIsGrounded(false), m_dGrav(12.0),
							m_dVelX(0.0), m_dVelY(0.0), m_dAccelX(0.0), m_dAccelY(0.0),
							m_dMaxVelX(6.0), m_dDrag(0.925)
{
	m_dMaxVelY = m_dGrav;
}

//not sure whether I'd use both these functions, but imma keep'em here for now
void Player::SetX(int x) { m_aSprite.SetX(x); }
void Player::SetY(int y) { m_aSprite.SetY(y); }

void Player::Kill() {
	if (m_aSprite.GetCurState() != DYING)
		m_aSprite.SetState(new SpriteState(DYING));
}

void Player::Update() {

	m_aSprite.Update();
	UpdateCollision();
}

void Player::Render() {

	m_aSprite.Render();

	//Will need to define the collision rect's 
	//boundaries and whatnot before rendering
	SDL_SetRenderDrawColor(RendererManager::Instance()->GetRenderer(), 128, 128, 255, 100);
	SDL_SetRenderDrawBlendMode(RendererManager::Instance()->GetRenderer(), SDL_BLENDMODE_ADD);
	SDL_RenderFillRect(RendererManager::Instance()->GetRenderer(), &m_rColl);
}

void Player::UpdateCollision() { //hard-coded offsets
	
	if (m_aSprite.GetCurState() == ROLLING) { 

		m_rColl.x = m_aSprite.GetDst().x + (m_aSprite.GetDst().w / 4);
		m_rColl.w = m_aSprite.GetDst().w / 2;
		m_rColl.y = m_aSprite.GetDst().y + (m_aSprite.GetDst().h / 2);
		m_rColl.h = m_aSprite.GetDst().h / 2;
	}
	else {

		m_rColl.x = m_aSprite.GetDst().x + (m_aSprite.GetDst().w / 5);

		if (m_aSprite.GetPlayer() == 1) { //GSonic has different collision offsets when not rolling
		
			m_rColl.w = 2 * m_aSprite.GetDst().w /3 ;
			m_rColl.y = m_aSprite.GetDst().y + (m_aSprite.GetDst().h/10);
			m_rColl.h = 9 * m_aSprite.GetDst().h * 10;
		}
		else {

			m_rColl.w = 2 * m_aSprite.GetDst().w / 4;
			m_rColl.y = m_aSprite.GetDst().y;
			m_rColl.h = m_aSprite.GetDst().h;
		}	
	}
}