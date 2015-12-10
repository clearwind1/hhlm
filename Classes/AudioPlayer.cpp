#include "AudioPlayer.h"
#include "UserData.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

static AudioPlayer *sharedAudioPlayer = NULL;

AudioPlayer *AudioPlayer::shared()
{
	if(sharedAudioPlayer == NULL)
		sharedAudioPlayer = new AudioPlayer();
	return sharedAudioPlayer;
}

AudioPlayer::AudioPlayer()
{
}

AudioPlayer::~AudioPlayer()
{
	if(sharedAudioPlayer != NULL)
	{
		delete sharedAudioPlayer;
		sharedAudioPlayer = NULL;
	}
}

void AudioPlayer::preloadMusic(const char *filePath)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(filePath);
}

void AudioPlayer::playMusic(const char *filePath, bool loop)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(filePath,loop);
	if(UserData::shared()->isMusicMute())
		CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void AudioPlayer::stopMusic(bool release)
{
	if(CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(release);
}

void AudioPlayer::pauseMusic()
{
	if(CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
		CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void AudioPlayer::resumeMusic()
{
	if(!UserData::shared()->isMusicMute())
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	}
}

void AudioPlayer::setMusicMute(bool mute)
{
	if(UserData::shared()->isMusicMute() != mute)
	{
		UserData::shared()->setMusicMute(mute);
		if(mute)
			pauseMusic();
		else
			resumeMusic();
	}
}

bool AudioPlayer::isMusicMute()
{
	return UserData::shared()->isMusicMute();
}

bool AudioPlayer::isMusicPlaying()
{
	return CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying();
}

void AudioPlayer::preloadEffect(const char *filePath)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(filePath);
}

void AudioPlayer::playEffect(const char* filePath, bool loop)
{
	if(!UserData::shared()->isEffectMute())
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(filePath, loop);
}

void AudioPlayer::stopEffect()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
}

void AudioPlayer::pauseEffect()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

void AudioPlayer::resumeEffect()
{
	if(!UserData::shared()->isEffectMute())
		CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}

void AudioPlayer::setEffectMute(bool mute)
{
	if(UserData::shared()->isEffectMute() != mute)
	{
		UserData::shared()->setEffectMute(mute);
		if(mute)
			pauseEffect();
		else
			resumeEffect();
	}
}

bool AudioPlayer::isEffectMute()
{
	return UserData::shared()->isEffectMute();
}

