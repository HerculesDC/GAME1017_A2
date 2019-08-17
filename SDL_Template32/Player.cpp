#include <algorithm>
#include "Player.hpp"
#include "Managers.hpp"

Player::Player(int index) : m_aSprite(index), m_bIsGrounded(false), m_dGrav(12.0),
							m_dVelX(0.0), m_dVelY(0.0), m_dAccelX(0.0), m_dAccelY(0.0),
							m_dMaxVelX(6.0), m_dDrag(0.925), m_fDirection(RIGHT)
{
	m_dMaxVelY = m_dGrav;
}

bool Player::Set(void* toState) {

	SpriteState* targetState = static_cast<SpriteState*>(toState);

	if (*targetState == m_aSprite.GetCurState()) return false;

	switch (*targetState) {
		//should check for impossible state transitions (later)
	case IDLING:
	case RUNNING:
	case JUMPING:
	case ROLLING:
	case DYING:
		m_aSprite.SetState(targetState);
		return true;
		break;
	default: return false;
	}
}

void Player::Update() {

	m_dAccelX = std::min(std::max(m_dAccelX, -m_dMaxAccelX), m_dMaxAccelX);
	m_dVelX = (m_dVelX + m_dAccelX) * m_dDrag;
	m_dVelX = std::min(std::max(m_dVelX, -m_dMaxVelX), m_dMaxVelX);
	m_aSprite.GetDstP()->x += (int)m_dVelX;

	// Y axis/jump stuff.
	if (!m_bIsGrounded) {
		m_dVelY = (m_dVelY + m_dAccelY) + m_dGrav / 10; // Applies downward force.
		m_dVelY = std::min(std::max(m_dVelY, -(m_dMaxVelY * 10)), (m_dMaxVelY));
	}
	
	/*
	UNUSED CODE THAT WILL STAY FOR NOW
	if (fabs(m_dVelY) > m_dGrav / 4) // So we go into jump when we move off a platform.
		Set(new SpriteState(JUMPING));
	*/

	m_aSprite.GetDstP()->y += (int)m_dVelY;

	m_aSprite.Update(); //there's a subtle coupling here between render and collision.
						//collision rectangle reads sprite position to adjust itself
	UpdateCollision();
}

void Player::Render() {

	m_aSprite.Render();

	//Will need to define the collision rect's 
	//boundaries and whatnot before rendering
	/*
	SDL_SetRenderDrawColor(RendererManager::Instance()->GetRenderer(), 128, 128, 255, 100);
	SDL_SetRenderDrawBlendMode(RendererManager::Instance()->GetRenderer(), SDL_BLENDMODE_ADD);
	SDL_RenderFillRect(RendererManager::Instance()->GetRenderer(), &m_rColl);
	*/
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
			m_rColl.h = 9 * m_aSprite.GetDst().h / 10;
		}
		else {

			m_rColl.w = 2 * m_aSprite.GetDst().w / 4;
			m_rColl.y = m_aSprite.GetDst().y;
			m_rColl.h = m_aSprite.GetDst().h;
		}	
	}
}