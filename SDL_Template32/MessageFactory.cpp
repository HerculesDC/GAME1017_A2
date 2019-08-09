#include "MessageFactory.hpp"
#include "Game.h"


MessageFactory* MessageFactory::Instance() {

	static MessageFactory* instance = new MessageFactory();
	return instance;
}

MessageFactory::MessageFactory() {

	TTF_Init();
	m_pFont = TTF_OpenFont("Img/caveman.ttf", 0x32);
	SetColor();
}

MessageFactory::compl MessageFactory() { TTF_CloseFont(m_pFont); }

bool MessageFactory::Init() { //sloppy coding, I know...
	return TTF_WasInit;
}

void MessageFactory::SetColor(int _r, int _g, int _b, int _a) {
	m_color.r = _r;
	m_color.g = _g;
	m_color.b = _b;
	m_color.a = _a;
}

SDL_Texture* MessageFactory::Export(const char* input) {

	SDL_Rect tempRect;
	tempRect.x = 50;
	tempRect.y = 200;

	TTF_SizeText(m_pFont, input, &tempRect.y, &tempRect.h);

	SDL_Surface* tempSurf = TTF_RenderText_Solid(m_pFont, input, m_color);
	SDL_Texture* tempTex = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), tempSurf);
	SDL_FreeSurface(tempSurf);

	return tempTex;
}