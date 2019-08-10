#pragma once
#include <functional>

#include "SDL.h"
#include "SDL_image.h"
#include "CommandHandler.hpp"

class Sprite {
	public:
		Sprite();
		virtual compl Sprite();
		virtual SDL_Rect GetSrc() const { return m_rSrc; }
		virtual SDL_Rect GetDst() const { return m_rDst; }
		
		//Reminder: for whatever reasons, 
		//		declaring these const doesnt work (study later)
		virtual SDL_Rect* GetSrcP() { return &m_rSrc; }
		virtual SDL_Rect* GetDstP() { return &m_rDst; }

		virtual void SetDest(SDL_Rect destRect);

		virtual void Update() = 0;
		virtual void Render() = 0;

	protected:
		//SOURCE refers to the source in the texture, 
		//whereas Destination refers to the destination on the window
		SDL_Rect m_rSrc, m_rDst;
};

enum ButtonState { MOUSEUP = 0, 
				   MOUSEDOWN = 1, 
				   MOUSEOVER = 1 << 1};

class Button : public Sprite {

	public:
		Button(Command& inCommand, bool (Command::*inFunction)(void*), void* param, int index = 0);
		compl Button();

		virtual void Update() override;
		virtual void Render() override;

	private:
		int m_iIndex; //for image retrieval purposes
		std::function<bool(Command&, void*)> OnClick;
		Command& m_cAddress;
		void* m_pParam;
		ButtonState m_innerState;
};