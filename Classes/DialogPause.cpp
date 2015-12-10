#include "DialogPause.h"
#include "UserData.h"
#include "AudioPlayer.h"
#include "LevelMap.h"
#include "LinkCard.h"
#include "Funtion.h"
#include "Shop.h"

USING_NS_CC;

#define DIALOGPAUSE_X_MARGIN	(0.1f)
#define DIALOGPAUSE_Y_MARGIN	(0.1f)

#define DIALOGPAUSE_Y_SPACE		(0.05f)


CCScene* DialogPause::scene()
{
    CCScene *scene = CCScene::create();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();  

	CCRenderTexture *renderTexture = CCRenderTexture::create(winSize.width, winSize.height);
	renderTexture->retain();
	CCScene *s = CCDirector::sharedDirector()->getRunningScene();
	renderTexture->begin();
	s->visit();
	renderTexture->end();

	CCSprite *sprite = CCSprite::createWithTexture(renderTexture->getSprite()->getTexture());
	sprite->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	sprite->setFlipY(true);
	sprite->setColor(ccGRAY);
	scene->addChild(sprite);

    DialogPause *layer = DialogPause::create();
    scene->addChild(layer);

    return scene;
}

bool DialogPause::init()
{
    //if(!DialogBase::init("images/dialog_pause_title.png", 0.85f, 0.4f, 0.05f))
    //    return false;
	if(!DialogBase::init("images/dialog_pause_title.png", NULL, 
		"images/dialog_button_back1.png", NULL, NULL,
		0.95f, 0.4f, 0.05f))
        return false;


	CCSize clientSize = DialogBase::getClientSize();
	CCPoint clientPos = DialogBase::getClientBasePoint();

	CCMenuItemImage *exitMenuItem = CCMenuItemImage::create(
		"images/dialog_pause_exit.png",
		NULL,
		this,
		menu_selector(DialogPause::mainMenuMenuCallback));
	CCSize exitMenuSize = exitMenuItem->getContentSize();
	CCMenuItemImage *restartMenuItem = CCMenuItemImage::create(
		"images/dialog_pause_restart.png",
		NULL,
		this,
		menu_selector(DialogPause::restartMenuCallback));
	CCSize restartMenuSize = restartMenuItem->getContentSize();
	CCMenuItemImage *continueMenuItem = CCMenuItemImage::create(
		"images/dialog_pause_continue.png",
		NULL,
		this,
		menu_selector(DialogPause::continueMenuCallback));
	CCSize continueMenuSize = continueMenuItem->getContentSize();
	
	float scale,height;
	float wScale = clientSize.width * (1 - DIALOGPAUSE_X_MARGIN * 2) / exitMenuSize.width;
	float hScale = clientSize.height * (1 - DIALOGPAUSE_Y_MARGIN * 2 - DIALOGPAUSE_Y_SPACE * 2) / 
		(exitMenuSize.height + restartMenuSize.height + continueMenuSize.height);
	if(wScale < hScale)
		scale = wScale;
	else
		scale = hScale;
	height = exitMenuSize.height * scale;
	float xStart = clientPos.x + clientSize.width / 2;
	float yStart = clientPos.y + clientSize.height - clientSize.height * DIALOGPAUSE_Y_MARGIN - height / 2;

	continueMenuItem->setScale(scale);
	continueMenuItem->setPosition(ccp(xStart, yStart));
	yStart -= height + clientSize.height * DIALOGPAUSE_Y_SPACE;

	restartMenuItem->setScale(scale);
	restartMenuItem->setPosition(ccp(xStart, yStart));
	yStart -= height + clientSize.height * DIALOGPAUSE_Y_SPACE;

	exitMenuItem->setScale(scale);
	exitMenuItem->setPosition(ccp(xStart, yStart));
	yStart -= height + clientSize.height * DIALOGPAUSE_Y_SPACE;

	CCMenu *menu = CCMenu::create(restartMenuItem, continueMenuItem, exitMenuItem, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu, 2);

	m_musicMute = AudioPlayer::shared()->isMusicMute();
	m_effectMute = AudioPlayer::shared()->isEffectMute();
	if(m_musicMute == false)
		AudioPlayer::shared()->pauseMusic();
	if(m_effectMute == false)
		AudioPlayer::shared()->pauseEffect();

	this->setKeypadEnabled(true);

	return true;
}

void DialogPause::mainMenuMenuCallback(CCObject* pSender)
{
	close();
	//CCDirector::sharedDirector()->popScene();
	UserData::shared()->flush();
	CCScene *scene = LevelMap::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(0.5,scene));
}

void DialogPause::continueMenuCallback(CCObject* pSender)
{
	close();
	LinkCard::shared()->resumeGame();
}

void DialogPause::restartMenuCallback(CCObject* pSender)
{
    if(readGameVirgour() == 0)
    {
        this->addChild(Shop::create(),100);
        return;
    }
	close();
	LinkCard::shared()->restartGame();
}

void DialogPause::close(void)
{
	AudioPlayer::shared()->resumeMusic();
	AudioPlayer::shared()->resumeEffect();

	CCDirector::sharedDirector()->popScene();
}

void DialogPause::closeClicked(void)
{
	continueMenuCallback(NULL);
}

