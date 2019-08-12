/* The idea behind this class is to encapsulate font management and provide other classes
 * with a standard way of generating text messages renderable in SDL. Since the TTF library
 * bases its design on creating textures with the message "printed" in them, I concluded it would
 * be easier to encapsulate this funtionality and let the TextureManager handle the rest
 */
#pragma once
#include "SDL.h"
#include "SDL_ttf.h"

class MessageFactory {
	public:
		static MessageFactory* Instance();
		bool Init() { return TTF_WasInit(); }
		void Release() { if (m_pFont) TTF_CloseFont(m_pFont); }

		SDL_Texture* Export(const char* input);

		void SetColor(int r = 0, int g = 0, int b = 0, int = SDL_ALPHA_OPAQUE);
		void SetColor(SDL_Color c);
		SDL_Color GetColor() const { return m_color; }
		TTF_Font* GetFont() const { return m_pFont; }

	private:
		MessageFactory();
		compl MessageFactory() { Release(); }

		TTF_Font* m_pFont;
		SDL_Color m_color;
};