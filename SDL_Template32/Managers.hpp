/* This file was designed with keeping all external resource
 *	management classes and functionality in mind. (except fonts)
 *	Most of them should be implemented as Singletons. That's why I eschewed inheritance
 *	(alternatives would involve templates and singletons, but that didn't work well => gotta study)
 *	I understand there should be some optimization mechanism for search and stuff
 *	But keep in mind that we're handling a very limited number of resources in this case
 *	Also, I've tried to avoid class redefinitions and forward declarations
 *	As far as I understand, the Game shouldn't need to know about specific resources such as Images
 *	and etc. So I moved the Image and Mixer headers here. The TextureManager is also responsible 
 *	for providing the textures to whoever needs it. I figured this "database" model made more sense
 *	than trying to have each class keeping track of their own textures directly
 */
#pragma once
#include <vector>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

class WindowManager {
	public:
		static WindowManager* Instance();
		compl WindowManager() { Release(); }

		SDL_Window* GetWindow() { return m_pWindow; }
		void Release() { if (m_pWindow) SDL_DestroyWindow(m_pWindow); }

	private:
		WindowManager();
		SDL_Window* m_pWindow;
};

class RendererManager {
	public:
		static RendererManager* Instance();
		compl RendererManager() { Release(); }

		SDL_Renderer* GetRenderer() { return m_pRenderer; }
		void Release() { if (m_pRenderer) SDL_DestroyRenderer(m_pRenderer); }

	private:
		RendererManager();
		SDL_Renderer* m_pRenderer;
};

class TextureManager {

	public:
		static TextureManager* Instance();
		bool Init();
		void Release();

		void Add(const char* filename);
		void Add(SDL_Texture* texture);
		SDL_Texture* Retrieve(int index) const;
		SDL_Rect* GetSize(int index, int x = 0, int y = 0) const;

	private:
		TextureManager();
		compl TextureManager();
		std::vector<SDL_Texture*> m_vTextures;
};

enum SoundStates { LOOP = -1, NO_LOOP, CHUNK_CHANNEL, };

class AudioManager {
	public:
		static AudioManager* Instance();
		bool Init();
		void Release();

		void AddMusic(const char* filename);
		void AddChunk(const char* filename);
		Mix_Music* GetMusic(int index) const;
		Mix_Chunk* GetChunk(int index) const;

	private:
		AudioManager();
		compl AudioManager();
		std::vector<Mix_Music*> m_vMusic;
		std::vector<Mix_Chunk*> m_vChunk;
};