/*This file was designed with keeping all external resource
	management classes and functionality in mind. (except fonts)
	Most of them should be implemented as Singletons. That's why I eschewed inheritance
	(alternatives would involve templates and singletons, but that didn't work well => gotta study)
	I understand there should be some optimization mechanism for search and stuff
	But keep in mind that we're handling a very limited number of resources in this case
	Also, I've tried to avoid class redefinitions and forwards declarations
	As far as I understand, the Game shouldn't need to know about specific resources such as Images and etc.
	So I moved the Image and Mixer headers here
 */

#pragma once
#include <vector>

#include "SDL_image.h"
#include "SDL_mixer.h"

class TextureManager {

	public:
		bool Init();
		void Add(const char* filename);
		SDL_Texture* Retrieve(int index) const;

		static TextureManager* Instance();

	private:
		TextureManager();
		compl TextureManager();
		std::vector<SDL_Texture*> m_vTextures;
};

class AudioManager {
	public:
		bool Init();
		void AddMusic(const char* filename);
		void AddChunk(const char* filename);
		Mix_Music* GetMusic(int index) const;
		Mix_Chunk* GetChunk(int index) const;

		static AudioManager* Instance();

	private:
		AudioManager();
		compl AudioManager();
		std::vector<Mix_Music*> m_vMusic;
		std::vector<Mix_Chunk*> m_vChunk;
};