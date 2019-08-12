#pragma once
#include <functional>

#include "SDL.h"
#include "CommandHandler.hpp"

class Sprite {
	public:
		//the Sprites (should) know where to retrieve their own images
		Sprite(int index = 0, SDL_Rect destination = { 0, 0, 0, 0 }, SDL_Rect source = { 0, 0, 0, 0 });
		virtual compl Sprite();

		virtual SDL_Rect GetSrc() const { return m_rSrc; }
		virtual SDL_Rect GetDst() const { return m_rDst; }
		
		virtual SDL_Rect* GetSrcP() { return &m_rSrc; }
		virtual SDL_Rect* GetDstP() { return &m_rDst; }

		virtual int GetIndex() const { return m_iIndex; }
		virtual SDL_Texture* GetTexture();

		virtual void SetSource(SDL_Rect srcRect);
		virtual void SetDest(SDL_Rect destRect);
		virtual void SetX(int i) { m_rDst.x = i; }

		virtual void SetSpeed(int i) {/*empty method for overriding*/}
		virtual bool SetState(void* exec) { /*TO OVERRIDE!*/ return false; }
		
		virtual void Update() {/*empty method for overriding*/ }
		virtual void Render();

	protected:
		SDL_Rect m_rSrc, m_rDst;
		const int m_iIndex;
};

enum ButtonState { MOUSEUP = 0, 
				   MOUSEDOWN = 1, 
				   MOUSEOVER = 1 << 1};

//Yes. This is little me, attempting big boy programming
class Button : virtual public Sprite {

	public:
		Button(Command& inCommand, bool (Command::*inFunction)(void*), void* param, 
			   int index = 0, bool bState = false);
		virtual compl Button();

		virtual void Update() override;
		virtual void Render() override;

	protected:
		bool m_bState;
		std::function<bool(Command&, void*)> OnClick;
		Command& m_cAddress;
		void* m_pParam;
		ButtonState m_innerState;
};

//The reason why I'm doing this is that I want to create an AnimatedButton,
//but Player only needs an AnimatedSprite as a component
//Thinking of turning this into an enum class later
enum SpriteState {IDLING, RUNNING, JUMPING, ROLLING, DYING,};

class AnimatedSprite : public virtual Sprite { //uses player's spritesheet
	public:
		AnimatedSprite(int sourceIndex = 5, int playerIndex = 0);
		virtual compl AnimatedSprite();

		virtual void Update() override;
		virtual void Render() override;
		
		//this one allows for "commandification" of the sprite
		virtual bool SetState(void* exec);
		virtual SpriteState GetCurState() const { return m_sCurState; }

	protected: //Unrelated classes shouldn't access Animate(), except through Update()
		virtual void Animate();

	protected:
		//Animated sprite initial info
		const int m_iSpriteSize;
		const int m_iSpriteBase; //0 for GSamus, 1 for GSonic, 3 for GBill (is this the guy from Contra?)

		//Animated sprite animation-specific info
		SpriteState m_sCurState;
		int m_iCurSprite,
			m_iMinSprite,
			m_iMaxSprite,
			m_iCurFrame,
			m_iMaxFrame;
};

class Background : public Sprite {
	public:
		Background(int index = 3, SDL_Rect destination = { 0, 0, 0, 0 }, 
				   SDL_Rect source = { 0, 0, 0, 0 }, int speed = 0);
		compl Background();
		
		virtual void SetDest(SDL_Rect destination) override;

		virtual void Update() override;
		virtual void Render() override;
		virtual void SetSpeed(int newSpeed) override;

	private:
		void Reset();
		int m_iSpeed, m_iStartX;
};