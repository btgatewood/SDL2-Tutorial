#include "main.h"

Mix_Chunk* sounds[NUM_SOUNDS]{ nullptr };
Mix_Music* music = nullptr;

void load_sounds()
{
	sounds[SND_PLAYER_FIRE] = Mix_LoadWAV("data/player_fire.ogg");
	sounds[SND_ALIEN_FIRE] = Mix_LoadWAV("data/alien_fire.ogg");
	sounds[SND_PLAYER_DEATH] = Mix_LoadWAV("data/player_death.ogg");
	sounds[SND_ALIEN_DEATH] = Mix_LoadWAV("data/alien_death.ogg");
}

void load_music(const char* file)
{
	if (music)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music = nullptr;
	}

	music = Mix_LoadMUS(file);
}

void play_music(bool loop)
{
	Mix_PlayMusic(music, loop ? -1 : 0);  // -1 == infinite loops, 0 == 0 loops
}

void play_sound(int id, int channel)
{
	Mix_PlayChannel(channel, sounds[id], 0);  // 0 loops
}
