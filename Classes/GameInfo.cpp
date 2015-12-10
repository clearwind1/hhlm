

#include "AppConfig.h"
#include "GameInfo.h"
#include "LinkCard.h"
#include "BackGround.h"

USING_NS_CC;

#define GAMEINFO_X_MARGIN			(GAMEINFO_X_WIDTH * 0.03f)
#define GAMEINFO_Y_MARGIN			(GAMEINFO_Y_HEIGHT * 0.05f)
#define GAMEINFO_Y_START			SCREEN_LINE4_START
#define GAMEINFO_Y_HEIGHT			SCREEN_LINE4_HEIGHT
#define GAMEINFO_X_START			SCREEN_GAMEINFO_START
#define GAMEINFO_X_WIDTH			SCREEN_GAMEINFO_WIDTH

#define GAMEINFO_Y1_START			SCREEN_LINE3_START
#define GAMEINFO_Y1_HEIGHT			SCREEN_LINE3_HEIGHT
#define GAMEINFO_SCORE_START		(0.2f)
#define GAMEINFO_SCORE_WIDTH		(0.5f)
#define GAMEINFO_COIN_START			(GAMEINFO_SCORE_START + GAMEINFO_SCORE_WIDTH)
#define GAMEINFO_COIN_WIDTH			(0.3)


#define GAMEINFO_SCALE_SIZE			(0.6f)
#define GAMEINFO_X_SPACE			(0.03f)

enum
{
	GameInfo1ItemLevel = 0,
	GameInfo1ItemTime,
	GameInfoItemCoin,
	GameInfo1ItemMax
};

enum
{
	GameInfo2ItemCoin = 0,
	GameInfo2ItemScore,
	GameInfo2ItemMax
};

static GameInfo *sharedGameInfo = NULL;

GameInfo *GameInfo::shared()
{
//	CCAssert(sharedGameInfo != NULL,"sharedGameInfo Invalid!!");
	return sharedGameInfo;
}

GameInfo::GameInfo()
{
	sharedGameInfo = this;
}

GameInfo::~GameInfo()
{
	sharedGameInfo = NULL;
}

bool GameInfo::initWithLevelAndTime()
{
	float width,height,wScale,hScale,scale,xStart,yStart;
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	width = winSize.width * (GAMEINFO_X_WIDTH - GAMEINFO_X_MARGIN * 2 - GAMEINFO_X_SPACE * (GameInfo1ItemMax - 1));
	height = winSize.height * (GAMEINFO_Y_HEIGHT - GAMEINFO_Y_MARGIN * 2) * GAMEINFO_SCALE_SIZE;

	xStart = winSize.width * (GAMEINFO_X_START + GAMEINFO_X_MARGIN);
	yStart = winSize.height * GAMEINFO_Y_START + winSize.height * GAMEINFO_Y_HEIGHT / 2;

	//关卡及背景
	CCSprite *levelSprite = CCSprite::create("images/game_info_level.png");
	CCSize levelSize = levelSprite->getContentSize();

	//关卡数,1-99
	m_levelDigitSprite = DigitSprite::create(0);
	m_levelDigitSprite->output("99");
	CCSize levelDigitSize = m_levelDigitSprite->getContentSize();

	//时间及背景
	CCSprite *clockSprite = CCSprite::create("images/game_info_time_clock.png");
	CCSize clockSize = clockSprite->getContentSize();

	//时间进度条
	CCSprite *timeProgBgSprite = CCSprite::create("images/game_info_time_progress_bg.png");
	CCSize timeProgBgSize = timeProgBgSprite->getContentSize();

	//时间数字显示
	m_timeDownDigitSprite = DigitSprite::create(0);
	m_timeDownDigitSprite->output("99:99");
	CCSize timeDownDigitSize = m_timeDownDigitSprite->getContentSize();

	wScale = width / (levelSize.width + clockSize.width + timeProgBgSize.width);
	hScale = height / levelSize.height;
	if(wScale < hScale)
		scale = wScale;
	else
		scale = hScale;
	levelSize = levelSize * scale;
	clockSize = clockSize * scale;
	timeProgBgSize = timeProgBgSize * scale;

	//关卡显示 
	levelSprite->setScale(scale);
	levelSprite->setPosition(ccp(xStart + levelSize.width / 2, yStart));
	this->addChild(levelSprite, 1);

	m_levelDigitSprite->setScale(levelSize.height / levelDigitSize.height * 0.9f);
	m_levelDigitSprite->setPosition(ccp(xStart + levelSize.width * 0.75f, yStart));
	this->addChild(m_levelDigitSprite, 2);
	xStart += levelSize.width + winSize.width * GAMEINFO_X_SPACE;

	//时间显示
	clockSprite->setScale(scale);
	clockSprite->setPosition(ccp(xStart + clockSize.width / 2, yStart));
	this->addChild(clockSprite, 1);

	m_timeDownDigitSprite->setScale(clockSize.height / timeDownDigitSize.height * 0.9f);
	m_timeDownDigitSprite->setPosition(ccp(xStart + clockSize.width  * 0.6f, yStart));
	this->addChild(m_timeDownDigitSprite, 2);
	xStart += clockSize.width + winSize.width * GAMEINFO_X_SPACE;

	timeProgBgSprite->setScale(scale);
	timeProgBgSprite->setPosition(ccp(xStart + timeProgBgSize.width / 2, yStart));
	this->addChild(timeProgBgSprite, 2);

	for(int i = 0; i < GameInfoTimeLevelMax; i++)
	{
		char buffer[128];
		snprintf(buffer,128,"images/game_info_time_progress_%d.png",i);
		m_timeProgSprite[i] = CCSprite::create((const char *)buffer);
		m_timeProgSprite[i]->retain();
	}

	m_timeProgBar = CCProgressTimer::create(m_timeProgSprite[GameInfoTimeLevelNormal]);
	m_timeProgBar->setScale(scale);
	m_timeProgBar->setType(kCCProgressTimerTypeBar);
	m_timeProgBar->setPosition(ccp(xStart + timeProgBgSize.width / 2, yStart));
	m_timeProgBar->setMidpoint(ccp(0, 0));
	m_timeProgBar->setBarChangeRate(ccp(1, 0));
	this->addChild(m_timeProgBar, 1);
	//xStart += timeProgBgSize.width + winSize.width * GAMEINFO_X_SPACE;

	return true;
}


bool GameInfo::initWithScore()
{
	float width,height,wScale,hScale,scale,xStart,yStart;
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	width = winSize.width * (GAMEINFO_SCORE_WIDTH - GAMEINFO_X_MARGIN * 2);
	height = winSize.height * (GAMEINFO_Y1_HEIGHT - GAMEINFO_Y_MARGIN * 2) * GAMEINFO_SCALE_SIZE;

	xStart = winSize.width * (GAMEINFO_SCORE_START + GAMEINFO_X_MARGIN);
	yStart = winSize.height * GAMEINFO_Y1_START + winSize.height * GAMEINFO_Y1_HEIGHT / 2;

	//分数进度条背景
	CCSprite *scoreProgBgSprite = CCSprite::create("images/game_info_score_progress_bg.png");
	CCSize scoreProgBgSize = scoreProgBgSprite->getContentSize();

	wScale = width / scoreProgBgSize.width;
	hScale = height / scoreProgBgSize.height;
	if(wScale < hScale)
		scale = wScale;
	else
		scale = hScale;
	scoreProgBgSize = scoreProgBgSize * scale;

	//分数进度条
	scoreProgBgSprite->setScale(scale);
	scoreProgBgSprite->setPosition(ccp(xStart + scoreProgBgSize.width / 2, yStart));
	this->addChild(scoreProgBgSprite, 1);

	for(int i = 0; i < GAMEINFO_SCORE_LEVEL_MAX; i++)
	{
		char buffer[128];
		snprintf(buffer,128,"images/game_info_score_progress_%d.png",i);
		m_scoreProgSprite[i] = CCSprite::create((const char *)buffer);
		m_scoreProgSprite[i]->retain();
		m_scoreProgSprite[i]->setScale(scale);
		m_scoreProgSprite[i]->setPosition(ccp(xStart + scoreProgBgSize.width / 2, yStart));
	}

	m_scoreProgBar = CCProgressTimer::create(m_scoreProgSprite[0]);
	m_scoreProgBar->setScale(scale);
	m_scoreProgBar->setType(kCCProgressTimerTypeBar);
	m_scoreProgBar->setPosition(ccp(xStart + scoreProgBgSize.width / 2, yStart));
	m_scoreProgBar->setMidpoint(ccp(0, 0));
	m_scoreProgBar->setBarChangeRate(ccp(1, 0));
	this->addChild(m_scoreProgBar, 2);
	m_scoreLevel = 0;
	m_scoreScore = NULL;
	setScoreLevel(0);

	return true;
}

bool GameInfo::initWithCoin()
{
	float width,height,wScale,hScale,scale,xStart,yStart;
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	width = winSize.width * (GAMEINFO_COIN_WIDTH - GAMEINFO_X_MARGIN * 2);
	height = winSize.height * (GAMEINFO_Y1_HEIGHT - GAMEINFO_Y_MARGIN * 2) * GAMEINFO_SCALE_SIZE;

	xStart = winSize.width * (GAMEINFO_COIN_START + GAMEINFO_X_MARGIN);
	yStart = winSize.height * GAMEINFO_Y1_START + winSize.height * GAMEINFO_Y1_HEIGHT / 2;

    
	//当前的钱币
	CCMenuItemImage *coinMenuItem = CCMenuItemImage::create(
		"images/game_info_coin_bg.png",
		NULL,
		this,
		menu_selector(GameInfo::coinMenuCallback));
	CCSize coinSize = coinMenuItem->getContentSize();

	//钱币数量
	m_coinDigitSprite = DigitSprite::create(0);
	m_coinDigitSprite->output("9999");
	CCSize coinDigitSize = m_coinDigitSprite->getContentSize();

	wScale = width / coinSize.width;
	hScale = height / coinSize.height;
	if(wScale < hScale)
		scale = wScale;
	else
		scale = hScale;
	coinSize = coinSize * scale;

	//钱币数量显示
	coinMenuItem->setScale(scale);
	coinMenuItem->setPosition(ccp(xStart + coinSize.width / 2, yStart));
	CCMenu *menu = CCMenu::create(coinMenuItem, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu, 1);

	m_coinDigitSprite->setScale(coinSize.height / coinDigitSize.height * 0.7f);
	m_coinDigitSprite->setPosition(ccp(xStart + coinSize.width / 2, yStart));
	this->addChild(m_coinDigitSprite, 2);
	m_coinDigitSprite->output("0");

	return true;
}

void GameInfo::coinMenuCallback(CCObject* pSender)
{
	LinkCard::shared()->gameStore(KTAGBUYGOLD);
}

bool GameInfo::init()
{
	if(!CCLayer::init())
		return false;
	if(!initWithLevelAndTime())
		return false;
//	if(!initWithCoin())
//		return false;
	if(!initWithScore())
		return false;
	return true;
}

void GameInfo::setTime(int time)
{
	if(time % 100 == 0)
	{
		m_timeDownDigitSprite->outputWithFormat("%02d:%02d",time / 100 / 60, time / 100 % 60);
	}

	int level = 0;
	float timePerc = (float)time / m_timeDown;
	if(timePerc > LINK_STAR_LEVLE_3)
		level = 2;
	else if(timePerc > LINK_STAR_LEVLE_2)
		level = 1;

	if(level == 0)
	{
		if(!BackGround::shared()->getRedColor())
			BackGround::shared()->setRedColor(true);
	}
	else
	{
		if(BackGround::shared()->getRedColor())
			BackGround::shared()->setRedColor(false);
	}

	if(m_timeProgSprite[level] != m_timeProgBar->getSprite())
		m_timeProgBar->setSprite(m_timeProgSprite[level]);
	m_timeProgBar->setPercentage(timePerc * 100.0f);
}

void GameInfo::setTime(int time,int timeDown)
{
	m_timeDown = timeDown;
	setTime(time);
}

void GameInfo::setScoreLevel(int level)
{
	if(level >= 0 && level <= GAMEINFO_SCORE_LEVEL_SIZE * GAMEINFO_SCORE_LEVEL_MAX)
	{
		if(level == 0)
		{
			if(m_scoreLevel != level)
			{
				if(m_scoreScore != NULL)
				{
					this->removeChild(m_scoreScore);
					m_scoreScore = NULL;
				}
				m_scoreProgBar->setSprite(m_scoreProgSprite[0]);
			}
			m_scoreLevel = level;
			m_scoreProgBar->setPercentage(0.0f);
		}
		else
		{
			int oldImage = (m_scoreLevel - 1) / GAMEINFO_SCORE_LEVEL_SIZE;
			int newImage = (level - 1) / GAMEINFO_SCORE_LEVEL_SIZE;
			if(newImage != oldImage)
			{
				if(m_scoreScore != NULL)
				{
					this->removeChild(m_scoreScore);
					m_scoreScore = NULL;
				}
				if(newImage != 0)
				{
					m_scoreScore = m_scoreProgSprite[newImage - 1];
					this->addChild(m_scoreScore, 1);
				}
				m_scoreProgBar->setSprite(m_scoreProgSprite[newImage]);
			}

			m_scoreLevel = level;
			int tempLevel = (level - 1) % GAMEINFO_SCORE_LEVEL_SIZE + 1;
			m_scoreProgBar->setPercentage(((float)tempLevel / GAMEINFO_SCORE_LEVEL_SIZE) * 100.0f);
		}
	}
}

GameInfo *GameInfo::create()
{
    GameInfo *pRet = new GameInfo();
    if(pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

void GameInfo::setLevel(int level)
{
	m_levelDigitSprite->output(level);
}

void GameInfo::setCoin(int coin)
{
	m_coinDigitSprite->output(coin, 0);
}

bool GameInfo::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	CCPoint point = pTouch->getLocationInView();
	point = CCDirector::sharedDirector()->convertToGL(point);


	return true;
}

void GameInfo::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority + 1, true);
}
