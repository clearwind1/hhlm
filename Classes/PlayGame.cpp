

#include "PlayGame.h"
#include "MainMenu.h"
#include "BackGround.h"
#include "GameMenu.h"
#include "GameInfo.h"

USING_NS_CC;

static PlayGame *playGameShared = NULL;

PlayGame *PlayGame::shared(void)
{
	CCAssert(playGameShared != NULL,"Error value");
	return playGameShared;
}

CCScene *PlayGame::scene(int level, bool resume)
{
    CCScene *scene = CCScene::create();
    
    PlayGame *playGame = PlayGame::create(level, resume);
	if(playGame == NULL)
		return NULL;

    scene->addChild(playGame);

	return scene;
}

bool PlayGame::init(int level, bool resume)
{
    if(!CCLayer::init())
        return false;

	CCLog("PlayGame::init");
	m_curLevel = level;
	addChild(BackGround::create(), 0);
	CCLog("PlayGame::init 111");
	addChild(GameInfo::create(), 2);
	CCLog("PlayGame::init 222");
	addChild(LinkCard::create(m_curLevel), 1);
	//CCLog("PlayGame::init 333");
	addChild(GameMenu::create(), 3);

	this->setTouchEnabled(true);
	this->setKeypadEnabled(true);

	return true;
}

PlayGame *PlayGame::create(int level, bool resume)
{
	PlayGame *playGame = new PlayGame();
	if(playGame != NULL && playGame->init(level, resume))
	{
		playGame->autorelease();
		playGameShared = playGame;
		return playGame;
	}
	else
	{
		CC_SAFE_DELETE(playGame);
		playGameShared = NULL;
		return NULL;
	}
}

bool PlayGame::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	return false;
}

void PlayGame::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority + 1, true);
}

void PlayGame::keyBackClicked(void)
{
	CCLog("keyBackClicked");
}

void PlayGame::keyMenuClicked(void)
{
	CCLog("keyMenuClicked");
}

void PlayGame::setLevelCallBack(float dt)
{
	if(LinkCard::shared() != NULL)
		this->removeChild(LinkCard::shared());
	addChild(LinkCard::create(m_curLevel, m_curOption), 1);
}

void PlayGame::setLevel(int level, int option)
{
	m_curLevel = level;
	m_curOption = option;
	this->scheduleOnce(schedule_selector(PlayGame::setLevelCallBack), 1.0f);
}
#if 0
void PlayGame::nextLevelCallBack(float dt)
{
	if(LinkCard::shared() != NULL)
		this->removeChild(LinkCard::shared());
	addChild(LinkCard::create(LinkCardInitModeNext),1);
}

void PlayGame::nextLevel(void)
{
	this->scheduleOnce(schedule_selector(PlayGame::nextLevelCallBack), 1.0f);
}

void PlayGame::restartLevelCallBack(float dt)
{
	if(LinkCard::shared() != NULL)
		this->removeChild(LinkCard::shared());
	addChild(LinkCard::create(LinkCardInitModeRestart),1);
}

void PlayGame::restartLevelWithDoubleTimeCallBack(float dt)
{
	if(LinkCard::shared() != NULL)
		this->removeChild(LinkCard::shared());
	addChild(LinkCard::create(LinkCardInitModeDoubleTime),1);
}

void PlayGame::restartLevel(void)
{
	this->scheduleOnce(schedule_selector(PlayGame::restartLevelCallBack), 1.0f);
}

void PlayGame::restartLevelWithDoubleTime(void)
{
	this->scheduleOnce(schedule_selector(PlayGame::restartLevelWithDoubleTimeCallBack), 1.0f);
}
#endif