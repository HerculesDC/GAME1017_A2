#include "Sprite.hpp"

//trying to avoid circular references here
#include "Game.h"
#include "Managers.hpp"
#include "CommandHandler.hpp"

Sprite::Sprite(int index, SDL_Rect dest, SDL_Rect src) : m_iIndex(index), m_rDst(dest), m_rSrc(src) {
	//sets destination first to make positioning easier within the constructor
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
	SDL_RenderCopy(Game::Instance()->GetRenderer(), TextureManager::Instance()->Retrieve(m_iIndex), nullptr, &m_rDst);
}

//requires the use of the functional header: check
Button::Button(Command& inCommand, bool (Command::*inFunction)(void*), void* param, int index, bool bState): 
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
		SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0x64, 0xFF, 0x32, 0x32);
		SDL_SetRenderDrawBlendMode(Game::Instance()->GetRenderer(), SDL_BLENDMODE_BLEND);
		SDL_RenderFillRect(Game::Instance()->GetRenderer(), &m_rDst);
	}
	Sprite::Render();
}

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
	SDL_RenderCopy(Game::Instance()->GetRenderer(), TextureManager::Instance()->Retrieve(m_iIndex), &m_rSrc, &m_rDst);
}