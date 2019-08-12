#include "Managers.hpp"

WindowManager* WindowManager::Instance() {
	static WindowManager* instance = new WindowManager();
	return instance;
}

//if config files are used, this file will need a parser
WindowManager::WindowManager() : 
	m_pWindow(SDL_CreateWindow("Gosh Dang to Heck! A Profanity-free Game", 
								SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
								1024, 768, 0))
{
	if (m_pWindow == nullptr) { /*flag error*/ }
}

RendererManager* RendererManager::Instance() {
	static RendererManager* instance = new RendererManager();
	return instance;
}

RendererManager::RendererManager() :
	m_pRenderer(SDL_CreateRenderer(WindowManager::Instance()->GetWindow(), -1, 0))
{
	if (m_pRenderer == nullptr) { /*flag error*/ }
}

TextureManager* TextureManager::Instance() {

	static TextureManager* instance = new TextureManager();
	return instance;
}

TextureManager::TextureManager() {

	m_vTextures.reserve(1);
}

TextureManager::compl TextureManager() {
	if (m_vTextures.size() != 0) Release();
}

bool TextureManager::Init() {
	return IMG_Init(IMG_INIT_PNG);
}

void TextureManager::Release() {
	if (!m_vTextures.empty()) {
		for (int i = 0; i < m_vTextures.size(); ++i) SDL_DestroyTexture(m_vTextures[i]);
	}
	m_vTextures.clear();
	m_vTextures.shrink_to_fit();
}

void TextureManager::Add(const char* filename)
{
	/* I know I mixed all steps and it becomes hard to debug, but I'm operating on a set of assumptions here
		Steps reminder: 
			create surface* with image, 
			create texture* from surface(and renderer), 
			push texture* to the vector.
	*/
	m_vTextures.push_back(SDL_CreateTextureFromSurface(RendererManager::Instance()->GetRenderer(), IMG_Load(filename)));
	//another comment on it: 
	//	this approach makes it (apparently) unnecessary to free the temporary surface after creation
}

void TextureManager::Add(SDL_Texture* source) {

	if (source) m_vTextures.push_back(source);
}

SDL_Rect* TextureManager::GetSize(int index, int x, int y) const {
	
	if (index < m_vTextures.size()) {
		SDL_Rect* temp = new SDL_Rect;
		temp->x = x;
		temp->y = y;
		SDL_QueryTexture(m_vTextures[index], NULL, NULL, &(*temp).w, &(*temp).h);
		return temp;
	}
	else return nullptr;
}

SDL_Texture* TextureManager::Retrieve(int index) const {
	if (index < m_vTextures.size()) return m_vTextures[index];
	else return nullptr;
}

#define MIX_STEREO_SOUND 2
#define MIX_AUDIO_SIZE 4096
#define MIX_AUDIO_CHANNELS 16

AudioManager* AudioManager::Instance() {

	static AudioManager* instance = new AudioManager();
	return instance;
}

AudioManager::AudioManager() {

	m_vChunk.reserve(1);
	m_vMusic.reserve(1);
}

AudioManager::compl AudioManager() {
	if (m_vChunk.size() != 0 || m_vMusic.size() != 0) Release();
}

bool AudioManager::Init() {

	if (Mix_Init(MIX_INIT_MP3) != 0) { //returns int
		Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_STEREO_SOUND, MIX_AUDIO_SIZE);
		Mix_AllocateChannels(MIX_AUDIO_CHANNELS); 
		return true;
	}
	return false;
}

void AudioManager::Release() {
	for (int i = 0; i < m_vChunk.size(); ++i) Mix_FreeChunk(m_vChunk[i]);
	for (int j = 0; j < m_vMusic.size(); ++j) Mix_FreeMusic(m_vMusic[j]);

	m_vChunk.clear();
	m_vMusic.clear();
	m_vChunk.shrink_to_fit();
	m_vMusic.shrink_to_fit();
}

void AudioManager::AddMusic(const char* filename) {
	m_vMusic.push_back(Mix_LoadMUS(filename));
}

void AudioManager::AddChunk(const char* filename) {
	m_vChunk.push_back(Mix_LoadWAV(filename));
}

Mix_Music* AudioManager::GetMusic(int index) const {
	if (index < m_vMusic.size()) return m_vMusic[index];
	return nullptr;
}

Mix_Chunk* AudioManager::GetChunk(int index) const {
	if (index < m_vChunk.size()) return m_vChunk[index];
	return nullptr;
}