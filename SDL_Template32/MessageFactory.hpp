#pragma once
#include "SDL.h"
#include "SDL_ttf.h"

//this is a small class, so I'm keeping it all in a single file
//will require work later to allow color changes
class MessageFactory {
	public:
		static MessageFactory* Instance();
		bool Init();

		SDL_Texture* Export(const char* input);

		void SetColor(int r = 0, int g = 0, int b = 0, int = SDL_ALPHA_OPAQUE);
		SDL_Color GetColor() const { return m_color; }
		TTF_Font* GetFont() const { return m_pFont; }

	private:
		TTF_Font* m_pFont;
		SDL_Color m_color;
		MessageFactory();
		compl MessageFactory();
};