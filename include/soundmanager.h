#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <PA9.h>
#include <parameters.h>

#define GAME_STATE_MENU 0
#define GAME_STATE_GAME 1
#define MAX_MOD 2

#define GAME_SOUND_GAMEOVER 0
#define GAME_SOUND_MOVE1 1
#define GAME_SOUND_MOVE2 2
#define GAME_SOUND_PATTERN1 3
#define GAME_SOUND_PATTERN2 4
#define GAME_SOUND_DISPOSE 5
#define MAX_SND 6

#define MAX_CHAN 4
#define FIRST_CHAN 16 - MAX_CHAN

class SoundManager
{
	public:
		SoundManager();
		~SoundManager();
		void playMod(int gs);
		void blockMooved();
		void patternCreated(bool first);
		void blockDisposed();
		void gameover();
		bool isMusicHearable();
		void switchMusicHearable();
        	static SoundManager& getInstance() {
        		static SoundManager instance;
			return instance;
		}
	private:
		void playSound(int channel, int gs);
		const u8* modMappings[MAX_MOD];
		const u8* sndMappings[MAX_SND];
		const u32* sndSizeMappings[MAX_SND];
		int chans[MAX_CHAN];
		int currentChan;
		void switchChan();
		bool musicHearable;
};

#endif
