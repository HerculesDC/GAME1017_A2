#include "Sprite.hpp"

//trying to avoid circular references here
#include "Game.h"
#include "Managers.hpp"
#include "CommandHandler.hpp"

Sprite::Sprite(int index, SDL_Rect src, SDL_Rect dest) : m_iIndex(index), m_rSrc(src), m_rDst(dest) {

}

Sprite::compl Sprite() {}

void Sprite::SetDest(SDL_Rect destRect) {

	m_rDst.x = destRect.x;
	m_rDst.y = destRect.y;
	m_rDst.w = destRect.w;
	m_rDst.h = destRect.h;
}

void Sprite::Render() {
	//CAUTIONARY: assumes rendering the whole sprite onto destination rectangle (hence source is nullptr
	//override accordingly
	SDL_RenderCopy(Game::Instance()->GetRenderer(), TextureManager::Instance()->Retrieve(m_iIndex), nullptr, &m_rDst);
}

//requires the use of the functional header: check
Button::Button(Command& inCommand, bool (Command::*inFunction)(void*), void* param, int index): 
	Sprite(index),
	m_cAddress(inCommand), 
	OnClick(inFunction),
	m_pParam(param), 
	m_innerState(MOUSEUP) 
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
				}
				if (CommandHandler::Instance()->GetMouseButton()->state == SDL_RELEASED) {
					//this is where the command associated to the button takes effect
					OnClick(m_cAddress, m_pParam);
				}
			}
			
		}
		else m_innerState = MOUSEUP;
		
	}
	//else m_innerState = MOUSEUP;
}

void Button::Render() {
	
	if (m_innerState!=MOUSEUP) {
		SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0x64, 0xFF, 0x32, 0x32);
		SDL_SetRenderDrawBlendMode(Game::Instance()->GetRenderer(), SDL_BLENDMODE_BLEND);
		SDL_RenderFillRect(Game::Instance()->GetRenderer(), &m_rDst);
	}
	Sprite::Render();
}

Background::Background(): m_iSpeed(0){}

Background::compl Background() {}

void Background::Update() {
	m_rDst.x -= m_iSpeed;
	//if (m_rDst.x <= -m_rDst.w)Reset();
}

void Background::Render() {
	//retrieves the Backgrounds image
	SDL_RenderCopy(Game::Instance()->GetRenderer(), TextureManager::Instance()->Retrieve(3), &m_rSrc, &m_rDst);
}