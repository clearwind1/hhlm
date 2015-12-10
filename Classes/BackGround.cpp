#include "BackGround.h"
#include "AudioPlayer.h"
#include "ActionInterval.h"


USING_NS_CC;

#define BACKGROUND_LAYER_COLOR_OPACITY		(100)

static BackGround *sharedBackGround = NULL;

BackGround *BackGround::shared()
{
	return sharedBackGround;
}

BackGround::BackGround()
{
	sharedBackGround = this;
}

BackGround::~BackGround()
{
	sharedBackGround = NULL;
}

bool BackGround::init()
{
    if(!CCNode::init())
        return false;

	float wScale,hScale,scale;
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite *sprite = CCSprite::create("images/bg_051.jpg");
	CCSize size = sprite->getContentSize();

	wScale = winSize.width / size.width;
	hScale = winSize.height / size.height;
	if(wScale > hScale)
		scale = wScale;
	else
		scale = hScale;
	sprite->setScale(scale);
	sprite->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	this->addChild(sprite,0);

    ccColor4B color4B = ccc4(255, 0, 0, BACKGROUND_LAYER_COLOR_OPACITY);
    m_layerColor = CCLayerColor::create(color4B, winSize.width, winSize.height);
    m_layerColor->setPosition(ccp(0, 0));
    ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE};
    m_layerColor->setBlendFunc(blendFunc);
	m_layerColor->setOpacity(0);
    this->addChild(m_layerColor);

	this->schedule(schedule_selector(BackGround::updateFireworks),1.0f);
	updateFireworks(0);

	m_isRedColor = false;
  
	return true;
}

void BackGround::updateFireworks(float dt)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCParticleSystemQuad *particle = CCParticleSystemQuad::create("particles/firework.plist");
	particle->setAutoRemoveOnFinish(true);
	particle->setBlendAdditive(false);
	float x = (rand() % (int)winSize.width) > (winSize.width / 2) ? winSize.width + 50 : -50;
	particle->setPosition(ccp(x,winSize.height - rand() % (int)(winSize.height / 4)));
	this->addChild(particle,2);
}

void BackGround::setRedColorCallBack()
{
	AudioPlayer::shared()->playEffect("sounds/effect_alarm.mp3");
}

void BackGround::setRedColor(bool value)
{
	if(value)
	{
		if(!m_isRedColor)
		{
			m_isRedColor = true;
			m_layerColor->setOpacity(0);
			CCActionInterval *delayTime = CCDelayTime::create(0.5f);
			CCActionInterval *fadeOut = FadeOut::create(0.3f, BACKGROUND_LAYER_COLOR_OPACITY);
			CCActionInterval *fadeIn = FadeIn::create(0.2f, BACKGROUND_LAYER_COLOR_OPACITY);
			CCCallFunc *funCall = CCCallFunc::create(this, callfunc_selector(BackGround::setRedColorCallBack));
			CCFiniteTimeAction *seq = CCSequence::create(funCall, fadeIn, delayTime, fadeOut, NULL);
			CCRepeatForever *repeatForever = CCRepeatForever::create((CCActionInterval *)seq);
			m_layerColor->runAction(repeatForever);
		}
	}
	else
	{
		m_isRedColor = false;
		m_layerColor->setOpacity(0);
		m_layerColor->stopAllActions();
	}
}

bool BackGround::getRedColor()
{
	return m_isRedColor;
}
