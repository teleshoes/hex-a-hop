/*
    Copyright (C) 2005-2007 Tom Beaumont

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include <list>
#include <SDL/SDL_mixer.h>
#include "sfx.h"

#ifndef DISABLE_SOUND
static int music_curr = -1;
static int music_next = -1;
static Mix_Music* music_chunks[HHOP_MUSIC_MAX];
static const char* const music_names[HHOP_MUSIC_MAX] =
{
	"music-ending.ogg",
	"music-game.ogg",
	"music-map.ogg",
	"music-title.ogg",
	"music-win.ogg"
};
static Mix_Chunk* sound_chunks[HHOP_SOUND_MAX];
static const char* const sound_names[HHOP_SOUND_MAX] =
{
	"sound-ui-fade.ogg",
	"sound-ui-menu.ogg",

	"sound-builder.ogg",
	"sound-death.ogg",
	"sound-collapse.ogg",
	"sound-disintegrate.ogg",
	"sound-explode-big.ogg",
	"sound-explode-small.ogg",
	"sound-floater-enter.ogg",
	"sound-floater-move.ogg",
	"sound-found-antiice.ogg",
	"sound-found-jump.ogg",
	"sound-ice.ogg",
	"sound-laser.ogg",
	"sound-lift-up.ogg",
	"sound-lift-down.ogg",
	"sound-spinner.ogg",
	"sound-step.ogg",
	"sound-trampoline.ogg",
	"sound-used-antiice.ogg",
	"sound-used-jump.ogg",
	"sound-win.ogg"
};
#endif

#ifndef DISABLE_SOUND
/* We store delayed sound effects in a queue and play them back when the
   time is right. It's sort of ugly but makes creating samples easier. */
class Sound
{
public:
	Sound(int ty, double ti) : type(ty), time(ti) { }
public:
	int type;
	double time;
};
std::list<Sound> sounds;
#endif

void InitSound()
{
#ifndef DISABLE_SOUND
	int i;
	extern String base_path;

	SDL_InitSubSystem(SDL_INIT_AUDIO);
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		fprintf(stderr, "Initializing audio failed: %s\n", Mix_GetError());
		exit(1);
	}
	for (i = 0 ; i < HHOP_MUSIC_MAX ; i++)
	{
		String file = base_path + "/" + music_names[i];
		music_chunks[i] = Mix_LoadMUS(file);
		if (!music_chunks[i])
			fprintf(stderr, "Cannot load music `%s': %s\n", (const char*) file, Mix_GetError());
	}
	for (i = 0 ; i < HHOP_SOUND_MAX ; i++)
	{
		String file = base_path + "/" + sound_names[i];
		sound_chunks[i] = Mix_LoadWAV(file);
		if (!sound_chunks[i])
			fprintf(stderr, "Cannot load music `%s': %s\n", (const char*) file, Mix_GetError());
	}
	Mix_AllocateChannels(HHOP_EFFECT_CHANNELS);
#endif
}

void FreeSound()
{
#ifndef DISABLE_SOUND
	int i;

	for (i = 0 ; i < HHOP_MUSIC_MAX ; i++)
	{
		if (music_chunks[i])
			Mix_FreeMusic(music_chunks[i]);
	}
	for (i = 0 ; i < HHOP_SOUND_MAX ; i++)
	{
		if (sound_chunks[i])
			Mix_FreeChunk(sound_chunks[i]);
	}
	Mix_CloseAudio();
#endif
}

void PlayMusic(int type)
{
#ifndef DISABLE_SOUND
	if (music_curr != type || music_next != type)
	{
		music_curr = -2;
		music_next = type;
		Mix_FadeOutMusic(HHOP_FADE_MUSIC_OUT);
	}
#endif
}

void PlaySound(int type)
{
#ifndef DISABLE_SOUND
	if (sound_chunks[type])
		Mix_PlayChannel(-1, sound_chunks[type], 0);
#endif
}

void QueueSound(int type, double time)
{
#ifndef DISABLE_SOUND
	sounds.push_back(Sound(type, time));
#endif
}

void UndoSound()
{
#ifndef DISABLE_SOUND
	sounds.clear();
#endif
}

void UpdateSound(double time)
{
#ifndef DISABLE_SOUND
	while (true)
	{
		std::list<Sound>::iterator i;

		// Find the first effect that needs playing.
		for (i = sounds.begin() ; i != sounds.end() ; i++)
		{
			if (time >= i->time)
			{
				PlaySound(i->type);
				break;
			}
		}

		// Erase the effect or stop if not found.
		if (i != sounds.end())
			sounds.erase(i);
		else
			break;
	}
	if (Mix_FadingMusic() == MIX_NO_FADING)
	{
		if (music_curr != music_next)
		{
			music_curr = music_next;
			if (music_chunks[music_curr])
				Mix_FadeInMusic(music_chunks[music_curr], -1, HHOP_FADE_MUSIC_IN);
		}
	}
#endif
}
