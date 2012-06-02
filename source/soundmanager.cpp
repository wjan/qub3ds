#include <soundmanager.h>
#include <stdlib.h>

#include <music1.h>
#include <music2.h>
#include <snd_gameover.h>
#include <snd_move1.h>
#include <snd_move2.h>
#include <snd_pattern1.h>
#include <snd_pattern2.h>
#include <snd_dispose.h>

SoundManager::SoundManager()
{
	PA_InitSound();
	modMappings[GAME_STATE_MENU] = music1;
	modMappings[GAME_STATE_GAME] = music2;

	sndMappings[GAME_SOUND_GAMEOVER] = snd_gameover;
	sndMappings[GAME_SOUND_MOVE1] = snd_move1;
	sndMappings[GAME_SOUND_MOVE2] = snd_move2;
	sndMappings[GAME_SOUND_PATTERN1] = snd_pattern1;
	sndMappings[GAME_SOUND_PATTERN2] = snd_pattern2;
	sndMappings[GAME_SOUND_DISPOSE] = snd_dispose;

	sndSizeMappings[GAME_SOUND_GAMEOVER] = snd_gameover_size;
	sndSizeMappings[GAME_SOUND_MOVE1] = snd_move1_size;
	sndSizeMappings[GAME_SOUND_MOVE2] = snd_move2_size;
	sndSizeMappings[GAME_SOUND_PATTERN1] = snd_pattern1_size;
	sndSizeMappings[GAME_SOUND_PATTERN2] = snd_pattern2_size;
	sndSizeMappings[GAME_SOUND_DISPOSE] = snd_dispose_size;

	for (int i = 0; i < MAX_CHAN; i++) {
		chans[i] = FIRST_CHAN + i;
	}
	currentChan = 0;
	
	musicHearable = true;
	PA_SetModVolume(SOUND_MOD_VOLUME);
}

void SoundManager::playMod(int gs) {
	PA_StopMod();
	if (gs >= 0 && gs < MAX_MOD) {
		const u8* toPlay = modMappings[gs];
		PA_PlayMod(toPlay);
	} else {
		// TODO: log out of available mod files range
	}
}

void SoundManager::playSound(int channel, int gs) {
	if (gs >= 0 && gs < MAX_SND) {
		PA_StopSound(channel);
		const u8* toPlay = sndMappings[gs];
		const u32* toPlay_size = sndSizeMappings[gs];
		PA_PlaySimpleSound(channel, toPlay);
	} else {
		// TODO: log out of available mod files range
	}
}

void SoundManager::blockMooved() {
	playSound(chans[currentChan], GAME_SOUND_MOVE1 + rand() % 2);
	switchChan();
}

void SoundManager::patternCreated(bool first) {
	int snd;
	if (first) {
		snd = GAME_SOUND_PATTERN1;
	} else {
		snd = GAME_SOUND_PATTERN2;
	}
	playSound(chans[currentChan], snd);
	switchChan();
}

void SoundManager::blockDisposed() {
	playSound(chans[currentChan], GAME_SOUND_DISPOSE);
	switchChan();
}

void SoundManager::switchChan() {
	currentChan++;
	if (currentChan >= MAX_CHAN) {
		currentChan = 0;
	}
}

void SoundManager::gameover() {
	playSound(chans[currentChan], GAME_SOUND_GAMEOVER);
	switchChan();
}

bool SoundManager::isMusicHearable() {
	return musicHearable;
}

void SoundManager::switchMusicHearable() {
	musicHearable = !musicHearable;
	if ( musicHearable ) {
		PA_SetModVolume(SOUND_MOD_VOLUME);
	} else {
		PA_SetModVolume(0);
	}
}

SoundManager::~SoundManager()
{
}
