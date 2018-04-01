#include "axeLib\SoundManager.h"

using namespace axe;

SoundManager::SoundManager()
{
	
}
SoundManager::~SoundManager()
{

}

void SoundManager::init(int num_samples)
{
	al_install_audio();
	al_init_acodec_addon();

	al_reserve_samples(num_samples);

	printf("SoundManager Initialized\n");
}
/*
Sound *SoundManager::loadSound(const char *path)
{
	Sound *sound = new Sound;

	sound->sample =	nullptr;
	sound->sample = al_load_sample(path);
	sound->path = path;

	if (sound->sample)
	{
		sound->id = cur_id;
		cur_id++;
		samples[sound->id] = sound;
		return sound;
	}
	else printf("Unable to load sample at path: %s\n", path);


	delete sound;
	return nullptr;
}
Music *SoundManager::loadMusic(const char *path)
{
	Music *music = new Music;
	music->sound = new Sound;

	music->vol = 0.0;
	music->instance = nullptr;
	music->sound->sample = nullptr;

	music->sound->sample = al_load_sample(path);

	if (music->sound->sample)
	{
		music->instance = nullptr;
		music->instance = al_create_sample_instance(music->sound->sample);
		music->sound->path = path;
		
		if (music->instance)
		{
			music->sound->id = cur_id;
			cur_id++;
			al_attach_sample_instance_to_mixer(music->instance, al_get_default_mixer());
			streams[music->sound->id] = music;
			return music;
		}
		else printf("Unable to create instance of path: %s\n", path);
	}
	else printf("Unable to load sample at path: %s\n", path);

	delete music->sound;
	delete music;
	return nullptr;
}

void SoundManager::playSound(Sound *sound, float vol)
{
	if (!v_sfx || !sound) return;

	al_play_sample(sound->sample, vol, 0, 1, ALLEGRO_PLAYMODE_ONCE, nullptr);
}

void SoundManager::playMusic(Music *music, float vol, ALLEGRO_PLAYMODE mod)
{
	if (!music) return;

	ALLEGRO_SAMPLE_INSTANCE *inst = streams[music->sound->id]->instance;

	if (!al_get_sample_instance_playing(inst))
	{
		setVolume(music, vol);
		if (!v_music) muteMusic();
		al_set_sample_instance_playmode(inst, mod);
		al_play_sample_instance(inst);
	}
}

void SoundManager::unloadAll()
{
	std::map<int, Sound*>::iterator it;

	for (it = samples.begin(); it != samples.end(); ++it)
	{
		if ((*it).second->sample)
		{
			printf("Destroying Sample: %s", (*it).second->path);
			al_destroy_sample((*it).second->sample);
		}
		delete (*it).second;
	}

	samples.clear();

	std::map<int, Music*>::iterator it2;

	for (it2 = streams.begin(); it2 != streams.end(); ++it2)
	{
		if ((*it2).second->instance)
		{
			printf("Destroying Sample Instance: %s", (*it2).second->sound->path);
			al_destroy_sample_instance((*it2).second->instance);
			al_destroy_sample((*it2).second->sound->sample);
		}

		delete (*it2).second->sound;
		delete (*it2).second;
	}

	streams.clear();
}

void SoundManager::unload(Sound *sound)
{
	if (sound == nullptr) return;

	int id = sound->id;

	if (samples[id]->sample)
		al_destroy_sample(samples[id]->sample); // if sample, destroy sample

	delete samples[id]; // delete object

	samples.erase(id);
}

void SoundManager::unload(Music *music)
{
	if (music == nullptr) return;

	int id = music->sound->id;

	if (streams[id]->instance)
	{
		al_destroy_sample_instance(streams[id]->instance);
		al_destroy_sample(streams[id]->sound->sample);
	}

	delete streams[id]->sound;
	delete streams[id];

	streams.erase(id);
}

void SoundManager::setVolume(Music *music, float val)
{
	if (!music || !music->instance) return;

	music->vol = val;

	if (v_music) al_set_sample_instance_gain(music->instance, val);

}

void SoundManager::stopPlaying(Music *music)
{
	if (!music || !music->instance) return;

	if (al_get_sample_instance_playing(music->instance))
	{
		al_set_sample_instance_playing(music->instance, false);
	}
}

void SoundManager::mute()
{
	muteSFX();
	muteMusic();
}

void SoundManager::unMute()
{
	unMuteSFX();
	unMuteMusic();
}

void SoundManager::muteSFX()
{
	v_sfx = false;

	al_stop_samples();
}

void SoundManager::muteMusic()
{
	v_music = false;

	std::map<int, Music*>::iterator it;

	for (it = streams.begin(); it != streams.end(); ++it)
	{
		if (!(*it).second->instance) continue;

		al_set_sample_instance_gain((*it).second->instance, 0.0);
	}
}

void SoundManager::unMuteSFX()
{
	v_sfx = true;
}

void SoundManager::unMuteMusic()
{
	if (v_music) return;

	v_music = true;

	std::map<int, Music*>::iterator it;

	for (it = streams.begin(); it != streams.end(); ++it)
	{
		if (!(*it).second->instance) continue;

		al_set_sample_instance_gain((*it).second->instance, (*it).second->vol);
	}
}*/