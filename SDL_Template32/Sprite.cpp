#include "Sprite.hpp"

//trying to avoid circular references here
#include "Managers.hpp"
#include "CommandHandler.hpp"

Sprite::Sprite(int index, SDL_Rect dest, SDL_Rect src) : m_iIndex(index), m_rDst(dest), m_rSrc(src) {
	//sets destination first to make positioning easier within the constructor. 
	//Source can be a zero rectangle by default
}

Sprite::compl Sprite() {}

void Sprite::SetSource(SDL_Rect srcRect) {

	m_rSrc.x = srcRect.x;
	m_rSrc.y = srcRect.y;
	m_rSrc.w = srcRect.w;
	m_rSrc.h = srcRect.h;
}

void Sprite::SetDest(SDL_Rect destRect) {

	m_rDst.x = destRect.x;
	m_rDst.y = destRect.y;
	m_rDst.w = destRect.w;
	m_rDst.h = destRect.h;
}

//implemented here because other classes shouldn't have unnecessary access to texture management.
//Ex: Obstacles indirectly retrieve that information through their sprites' indices
SDL_Texture* Sprite::GetTexture() { return TextureManager::Instance()->Retrieve(m_iIndex); }

void Sprite::Render() {
	//CAUTIONARY: assumes rendering the whole sprite onto destination rectangle (hence source is nullptr)
	//override accordingly
	SDL_RenderCopy(RendererManager::Instance()->GetRenderer(), TextureManager::Instance()->Retrieve(m_iIndex), nullptr, &m_rDst);
}

//requires the use of the functional header: check
Button::Button(Command& inCommand, bool (Command::*inFunction)(void*), void* param, bool bState, int index):
	Sprite(index),
	m_cAddress(inCommand), 
	OnClick(inFunction),
	m_pParam(param), 
	m_innerState(MOUSEUP),
	m_bState(bState)
{
}

Button::compl Button() {}

void Button::Update() {

	if (CommandHandler::Instance()->GetMouse() != nullptr) {
	
		if (SDL_PointInRect(CommandHandler::Instance()->GetMouse(), &m_rDst)) {

			m_innerState = MOUSEOVER;
			if (CommandHandler::Instance()->GetMouseButton() != nullptr) {
				if (CommandHandler::Instance()->GetMouseButton()->state == SDL_PRESSED) {
					m_innerState = MOUSEDOWN;
					m_bState = true;
				}
				if (CommandHandler::Instance()->GetMouseButton()->state == SDL_RELEASED) {
					//this is where the command associated to the button takes effect
					//Note: function objects retain their "called state",
					//		hence the need of a flag
					if (m_bState) {
						OnClick(m_cAddress, m_pParam);
						m_bState = false;
					}
				}
			}
		}
		else m_innerState = MOUSEUP;
	}
}

void Button::Render() {
	
	if (m_innerState!=MOUSEUP) {
		SDL_SetRenderDrawColor(RendererManager::Instance()->GetRenderer(), 0x64, 0xFF, 0x32, 0x32);
		SDL_SetRenderDrawBlendMode(RendererManager::Instance()->GetRenderer(), SDL_BLENDMODE_BLEND);
		SDL_RenderFillRect(RendererManager::Instance()->GetRenderer(), &m_rDst);
	}
	Sprite::Render();
}

AnimatedSprite::AnimatedSprite(int playerChoice, int index) :
	Sprite(index),
	m_iSpriteBase(playerChoice % 3), //ensures it's a valid part of the sprite
	m_sCurState(IDLING), 
	m_iSpriteSize(128)
{
	//I'm aware it's inherited, but using a function call felt clearer
	SetSource({0, 2*m_iSpriteBase*m_iSpriteSize, m_iSpriteSize, m_iSpriteSize});
}

AnimatedSprite::compl AnimatedSprite() {}

void AnimatedSprite::Animate() {

	m_iCurFrame++;
	if (m_iCurFrame == m_iMaxFrame) {

		m_iCurFrame = 0;
		m_iCurSprite++;

		if (m_iCurSprite == m_iMaxSprite)
			m_iCurSprite = m_iMinSprite;
	}
	m_rSrc.x = m_rSrc.w * m_iCurSprite;
}

bool AnimatedSprite::SetState(void* state) {
	
	SpriteState* targetState = static_cast<SpriteState*>(state);

	m_iCurFrame = 0; //took this assignment as it's common to all states

	//OBS: MaxFrames are halved due to halved framerate
	switch (*targetState) {
		case IDLING:
			m_sCurState = IDLING;
			m_iMaxFrame = 1;
			m_iCurSprite = m_iMinSprite = 0;
			m_iMaxSprite = 1;
			m_rSrc.y = m_iSpriteBase * 2 * m_iSpriteSize;
			return true;
		case RUNNING:
			m_sCurState = RUNNING;
			m_iMaxFrame = 3;
			m_iCurSprite = m_iMinSprite = 0;
			m_iMaxSprite = 8;
			m_rSrc.y = m_iSpriteBase * 2 * m_iSpriteSize;
			return true;
		case JUMPING:
			m_sCurState = JUMPING;
			m_iMaxFrame = 1;
			m_iCurSprite = m_iMinSprite = 8;
			m_iMaxSprite = 9;
			m_rSrc.y = m_iSpriteBase * 2 * m_iSpriteSize;
			return true;
		case ROLLING:
			m_sCurState = ROLLING;
			m_iMaxFrame = 3;
			m_iCurSprite = m_iMinSprite = 0;
			m_iMaxSprite = 4;
			m_rSrc.y = m_iSpriteSize + (m_iSpriteBase * 2 * m_iSpriteSize);
			return true;
		case DYING:
			m_sCurState = DYING;
			m_iMaxFrame = 6;
			m_iCurSprite = m_iMinSprite = 4;
			m_iMaxSprite = 9;
			//there's a counter for this one, but I'm not 100% sure why... yet
			m_rSrc.y = m_iSpriteSize + (m_iSpriteBase * 2 * m_iSpriteSize);
			return true;
		default:
			return false;
	}
	return false;
}

void AnimatedSprite::Update() {
	//not sure whether there should be other stuff here for now
	Animate();
}

void AnimatedSprite::Render() {
	SDL_RenderCopy(RendererManager::Instance()->GetRenderer(), TextureManager::Instance()->Retrieve(m_iIndex), &m_rSrc, &m_rDst);
}

AnimatedButton::AnimatedButton(Command& inCommand, bool (Command::*inFunction)(void*), void* param,
	bool buttonState, int playerIndex, int sourceIndex) :
	Button(inCommand, inFunction, param, buttonState, sourceIndex),
	AnimatedSprite(playerIndex, sourceIndex),
	Sprite(sourceIndex)
{/*One mystery remains, however: which index is considered when constructing the base Sprite?*/}

AnimatedButton::compl AnimatedButton() {}

void AnimatedButton::Update() {

	if (CommandHandler::Instance()->GetMouse() != nullptr) {

		if (SDL_PointInRect(CommandHandler::Instance()->GetMouse(), &m_rDst)) {
			
			if (m_sCurState != RUNNING)
				SetState(new SpriteState(RUNNING));
			
			m_innerState = MOUSEOVER;
			
			if (CommandHandler::Instance()->GetMouseButton() != nullptr) {
				if (CommandHandler::Instance()->GetMouseButton()->state == SDL_PRESSED) {
			
					m_innerState = MOUSEDOWN;
					m_bState = true;
				}
				if (CommandHandler::Instance()->GetMouseButton()->state == SDL_RELEASED) {
					if (m_bState) {
					
						OnClick(m_cAddress, m_pParam);
						m_bState = false;
					}
				}
			}
		}
		else { 

			if (m_sCurState != IDLING)
				SetState(new SpriteState(IDLING));

			m_innerState = MOUSEUP;
		}
	}
	Animate();
}

void AnimatedButton::Render() { AnimatedSprite::Render(); }

Background::Background(int index, SDL_Rect destination, SDL_Rect source, int speed) 
					  : Sprite(index, destination, source), 
						m_iSpeed(speed), m_iStartX(destination.x)
{}

Background::compl Background() {}

void Background::SetDest(SDL_Rect destination) {
	Sprite::SetDest(destination);
	m_iStartX = destination.x;
}

void Background::SetSpeed(int newSpeed) { m_iSpeed = newSpeed; }

void Background::Update() {
	m_rDst.x -= m_iSpeed;
	if (m_rDst.x <= m_iStartX-m_rDst.w) Reset();
}

void Background::Reset() { m_rDst.x = m_iStartX; }

void Background::Render() {
	//retrieves the Backgrounds image
	SDL_RenderCopy(RendererManager::Instance()->GetRenderer(), TextureManager::Instance()->Retrieve(m_iIndex), &m_rSrc, &m_rDst);
}