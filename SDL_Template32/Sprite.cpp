#include "Sprite.hpp"

//trying to avoid circular references here
#include "Game.h"
#include "Managers.hpp"

Sprite::Sprite() :m_rSrc({ 0, 0, 0, 0 }), m_rDst({ 0, 0, 0, 0 }) {

}

Sprite::compl Sprite() {}

void Sprite::SetDest(SDL_Rect destRect) {

	m_rDst.x = destRect.x;
	m_rDst.y = destRect.y;
	m_rDst.w = destRect.w;
	m_rDst.h = destRect.h;
}


//requires the use of the functional header: check
Button::Button(Command& inCommand, bool (Command::*inFunction)(void*), void* param, int index): 
	m_cAddress(inCommand), 
	OnClick(inFunction),
	m_pParam(param), 
	m_iIndex(index), 
	m_innerState(MOUSEUP) 
{
}

Button::compl Button() {}

#include "CommandHandler.hpp"
void Button::Update() {

	if (CommandHandler::Instance()->GetMouse() != nullptr) {
	
		if (CommandHandler::Instance()->GetMouse()->x >= m_rDst.x &&
			CommandHandler::Instance()->GetMouse()->x <= m_rDst.x + m_rDst.w &&
			CommandHandler::Instance()->GetMouse()->y >= m_rDst.y &&
			CommandHandler::Instance()->GetMouse()->y <= m_rDst.y + m_rDst.h) {

			m_innerState = MOUSEOVER;
			/*
			if (CommandHandler::Instance()->GetMouse()->state == SDL_PRESSED) {
				m_innerState = MOUSEDOWN;
			}
			if (CommandHandler::Instance()->GetMouse()->state == SDL_RELEASED) {
				//this is where the command associated to the button takes effect
				OnClick(m_cAddress, m_pParam);
			}
			//*/
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
	
	 //Retrieve() takes the index of the desired texture
	SDL_RenderCopy(Game::Instance()->GetRenderer(), TextureManager::Instance()->Retrieve(m_iIndex), nullptr, &m_rDst);
}