#pragma once

#include <string>
#include <allegro5\allegro.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>

namespace axe
{

	/*struct Sound
	{
		ALLEGRO_SAMPLE *sample;
		int id;
		const char * path;
	};

	struct Music
	{
		ALLEGRO_SAMPLE_INSTANCE *instance;
		Sound *sound;
		float vol;
	};*/

	class SoundManager
	{
	public:
		SoundManager(void);
		~SoundManager();

		void init(int num_samples);

	private:
		bool b_sfx;
		bool b_music;

		float f_sfx_volume;
		float f_music_volume;
	};

};