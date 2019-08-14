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

		//In retrospect, this should've been a sign that I ought to have reworked something...
		virtual void SetSpeed(int i) {/*empty method for overriding*/}
		virtual bool SetState(void* exec) { /*TO OVERRIDE!*/ return false; }
		
		virtual void Update() {/*empty method for overriding*/ }
		virtual void Render(); //NOT EMPTY

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
		Button(Command& inCommand, bool (Command::* inFunction)(void*), void* param, 
			   bool bState = false, int index = 0);
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

class AnimatedSprite : public virtual Sprite { //uses player's spritesheet by default
	public:
		AnimatedSprite(int playerIndex = 0, int sourceIndex = 7);
		virtual compl AnimatedSprite();

		virtual void Update() override;
		virtual void Render() override;
		
		//this one allows for "commandification" of the sprite
		virtual bool SetState(void* exec);
		virtual SpriteState GetCurState() const { return m_sCurState; }
		virtual int GetPlayer() const { return m_iSpriteBase; }

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

class AnimatedButton : public virtual Button, virtual public AnimatedSprite {
	public:
		//OBS: Because commands cannot be defaulted, Button parameters must come first in this case
		AnimatedButton(Command& inCommand, bool (Command::* inFunction)(void*), void* param, 
						bool buttonState = false, int playerIndex = 0, int sourceIndex = 7);
		compl AnimatedButton();
	
		//OBS: Classes with multiple inheritance MUST override methods common to both parents
		virtual void Update() final override;
		virtual void Render() final override;

	private:
		//may even be unneedded, because everything else is in parent classes
};

class Background : public Sprite {
	public:
		Background(int index = 5, SDL_Rect destination = { 0, 0, 0, 0 }, 
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