#ifndef __AUDIO_PLAYER_H__	
#define __AUDIO_PLAYER_H__


#include "cocos2d.h"


class AudioPlayer
{
public:
	AudioPlayer();
	~AudioPlayer();

	static AudioPlayer *shared();

	void preloadMusic(const char *filePath);
	void playMusic(const char *filePath, bool loop = false);
	void stopMusic(bool release = false);
	void pauseMusic();
	void resumeMusic();
	void setMusicMute(bool mute);
	bool isMusicMute();
	bool isMusicPlaying();

	void preloadEffect(const char *filePath);
	void playEffect(const char *filePath, bool loop = false);
	void stopEffect();
	void pauseEffect();
	void resumeEffect();
	void setEffectMute(bool mute);
	bool isEffectMute();

};


#endif //__AUDIO_PLAYER_H__