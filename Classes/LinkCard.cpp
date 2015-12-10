
#include "LinkCard.h"
#include "AudioPlayer.h"
#include "MainMenu.h"
#include "UserData.h"
#include "AppConfig.h"
#include "PlayGame.h"
#include "DialogPause.h"
#include "BackGround.h"
#include "GameInfo.h"
#include "DialogGameEnd.h"
#include "DialogTimeout.h"
//#include "DialogBuyGift.h"
#include "GameMenu.h"
#include "Funtion.h"

USING_NS_CC;


#define LINKCARD_SCALE			(0.9f)
#define LINKCARD_SEL_SIZE		(1.05f)

#define LINKCARD_X_MARGIN		(0.02f)
#define LINKCARD_Y_MARGIN		(0.01f)

#define LINKCARD_X_START		(0.0f)
#define LINKCARD_X_WIDTH		(1.0f)
#define LINKCARD_Y_START		SCREEN_LINE2_START
#define LINKCARD_Y_HEIGHT		SCREEN_LINE2_HEIGHT

#define LINKCARD_XLINK_MAX		(m_xCards + 2)
#define LINKCARD_YLINK_MAX		(m_yCards + 2)

#define LINKCARD_SCORE_COUNT_MAX	(10)

#define LINKCARD_INVALID_ID		(0)
#define LINKCARD_FREEZE_STATUS	(-1)
#define LINKCARD_DISABLE_STATUS	(-2)

#define LINKCARD_ZORDER_BASE	(100)
#define LINKCARD_ZORDER_MAX		(LINKCARD_ZORDER_BASE + 100)

enum
{
	LinkCardTagMin = 100,
	LinkCardTagGameTimeout,
	LinkCardTagGameEnd,
	LinkCardTagBuyGift,
	LinkCardTagMax
};

static LinkCard *linkCardShared = NULL;

LinkCard *LinkCard::shared(void)
{
	//CCAssert(linkCardShared != NULL,"Error value");
	return linkCardShared;
}

LinkCard::~LinkCard()
{
    linkCardShared = NULL;
}

void LinkCard::initWithLayout(void)
{
	float width,height,wScale,hScale;
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	m_xCards = m_cardMap->getWidth();
	m_yCards = m_cardMap->getHeight();

	width = winSize.width * (LINKCARD_X_WIDTH - LINKCARD_X_MARGIN * 2);
	height = winSize.height * (LINKCARD_Y_HEIGHT - LINKCARD_Y_MARGIN * 2);
	wScale = width / (m_xCards + 1) / CardSprite::getCardWidth();
	hScale = height / (m_yCards + 1) / CardSprite::getCardHeight();
	if(wScale < hScale)
		m_scale = wScale;
	else
		m_scale = hScale;
	m_width = CardSprite::getCardWidth() * m_scale;
	m_height = CardSprite::getCardHeight() * m_scale;

	m_xStart = winSize.width * LINKCARD_X_START + 
		(winSize.width - m_width * m_xCards) / 2;
	m_yStart = winSize.height * (LINKCARD_Y_START + LINKCARD_Y_HEIGHT) - 
		(winSize.height * LINKCARD_Y_HEIGHT - m_height * m_yCards) / 2;
}

bool LinkCard::initWithLink(void)
{
	m_count = m_cardMap->getCardCount();
	//m_cardStyle = 2;
	//m_cardStyle = rand() % (CardSpriteStyleMax - 3) + 2;
	m_cardStyle = rand() % (CardSpriteStyleEnd - CardSpriteStyleStart + 1) + CardSpriteStyleStart;

	CCAssert(m_count > 0, "Invalid Map Table");
    CCAssert(m_count % 2 == 0, "Invalid Map Table");

// 	if(m_count <= 0 || m_count % 2 != 0)
// 		return false;

	CardSprite **cardSprite = new CardSprite *[m_count];

	m_addCount = m_count;

	int index,temp = m_count / 2;
	for(index = 0; index < temp; index++)
	{
		int id = rand() % CardSprite::getCardTotal(m_cardStyle) + 1;
		cardSprite[index] = CardSprite::create(m_cardStyle,id);
	}
	for(index = 0; index < temp; index++)
	{
		cardSprite[index + temp] = CardSprite::create(m_cardStyle,cardSprite[index]->getID());
	}

	index = 0;
	for(int y = 0;y < m_yCards;y++)
	{
		for(int x = 0;x < m_xCards;x++)
		{
			int i = y * m_xCards + x;
			m_linkSprite[i] = NULL;
			int cardType = m_cardMap->getCardType(i);

			if(cardType == LinkMapTypeDisable)
				m_linkSprite[i] = CardSprite::create(CardSpriteStyleSpecial,CardSpriteSpecialIdDisable);
			else if(cardType == LinkMapTypeNormal || cardType == LinkMapTypeFreeze)
				m_linkSprite[i] = cardSprite[index++];
			else
				continue;

			m_linkSprite[i]->x = x;
			m_linkSprite[i]->y = y;
			m_linkSprite[i]->setScale(m_scale * LINKCARD_SCALE);
			m_linkSprite[i]->setPosition(getPos(x,y));
			if(cardType == LinkMapTypeNormal || cardType == LinkMapTypeFreeze)
				m_linkSprite[i]->setHide(true);
			if(cardType == LinkMapTypeFreeze)
				m_linkSprite[i]->setFreeze(true);
			this->addChild(m_linkSprite[i], i + LINKCARD_ZORDER_BASE);
		}
	}

	delete[] cardSprite;

	m_lock = false;
	m_clearLock = false;
	refreshCard(false);
	m_lock = true;

	return true;
}

void LinkCard::gameStartCallBack(float dt)
{
	for(int index = 0; index < m_xCards * m_yCards; index++)
	{
		if(m_linkSprite[index] != NULL)
		{
			int style = m_linkSprite[index]->getStyle();
			if(style >= CardSpriteStyleStart && style <= CardSpriteStyleEnd)
				m_linkSprite[index]->setHide(false, 0.5f);
		}
	}
	this->scheduleOnce(schedule_selector(LinkCard::gameGoStart), 0.5f);
}

void LinkCard::gameGoStart(float dt)
{
	CCLog("LinkCard::gameGoStart");
	this->schedule(schedule_selector(LinkCard::timeUpdateCallBack), 0.01f);
	m_lock = false;
}

void LinkCard::timeUpdateCallBack(float dt)
{
	m_timeDown--;
	m_timeCount++;
	//CCLog("LinkCard::timeUpdateCallBack");
	GameInfo::shared()->setTime(m_timeDown);
	if(m_timeDown == 0)
	{
		//TODO:Game Over
		this->unschedule(schedule_selector(LinkCard::timeUpdateCallBack));
		if(m_count != 0)
			goLevelEnd(false);
	}
}

bool LinkCard::init(int level, int mode)
{
	if(!CCLayer::init())
		return false;

	m_lock = true;
	m_linkLineCount = 0;
	m_linkSel = NULL;
	m_linkHelp1 = NULL;
	m_linkHelp2 = NULL;
	m_clearLink = NULL;
	m_clearLink2 = NULL;

	if(mode == LinkCardInitModeDefault)
		mode = LinkCardInitModeNew;

	bool isDoubleTime = false;
	if(mode == LinkCardInitModeDoubleTime)
	{
		isDoubleTime = true;
		mode = LinkCardInitModeRestart;
	}

	CCLog("LinkCard::init");
	if(level < 1)
		level = 1;
	if(level > LINK_LEVEL_MAX)
		level = LINK_LEVEL_MAX;
	m_level = level;
	m_style = m_level;

	m_cardMap = LinkMap::create(m_style);
	m_cardMap->retain();
//	m_cardMap->retain();

	initWithLayout();

	if(!initWithLink())
		return false;

	m_timeDownMax = m_cardMap->getTime() * 100;	//0.01 seconds
    
    CCLog("m_time=======%d",m_timeDownMax);
    
	if(isDoubleTime)
	{
		m_timeDownMax *= 2;
	}
	m_timeDown = m_timeDownMax;
	m_timeCount = 0;

	m_lightning = Lightning::create(this, LINKCARD_ZORDER_BASE - 1, LINKCARD_ZORDER_MAX);
	this->addChild(m_lightning, LINKCARD_ZORDER_MAX);

	CCLog("LinkCard::init 1111");
	GameInfo::shared()->setTime(m_timeDown,m_timeDown);
	GameInfo::shared()->setScoreLevel(0);
	GameInfo::shared()->setLevel(m_level);
	//GameInfo::shared()->setCoin(UserData::shared()->getGoldCoins());
	BackGround::shared()->setRedColor(false);

	m_scoreLevel = 0;
	m_scoreCount = LINKCARD_SCORE_COUNT_MAX + 1;
	this->schedule(schedule_selector(LinkCard::scoreLevelHandler), 0.1f);

	this->scheduleOnce(schedule_selector(LinkCard::gameStartCallBack), 2.0f);

	goLevelStart();

	this->setTouchEnabled(true);
	this->setKeypadEnabled(true);

	return true;
}

LinkCard *LinkCard::create(int level, int mode)
{
	LinkCard *linkCard = new LinkCard();
	if(linkCard != NULL && linkCard->init(level, mode))
	{
		linkCard->autorelease();
		linkCardShared = linkCard;
		return linkCard;
	}
	else
	{
		CC_SAFE_DELETE(linkCard);
		linkCardShared = NULL;
		return NULL;
	}
}

void LinkCard::goLevelStart(void)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	CCSprite *goLevelSprite = CCSprite::create("images/game_go_level_bg.png");
	CCSize goLevelSize = goLevelSprite->getContentSize();
	float scale = winSize.width / goLevelSize.width;
	goLevelSprite->setScale(scale);
	goLevelSprite->setPosition(ccp(winSize.width / 2,winSize.height + goLevelSize.height));
	this->addChild(goLevelSprite, 201 + LINKCARD_ZORDER_MAX);

	DigitSprite *levelDigitSprite = DigitSprite::create(0);
	levelDigitSprite->output(m_level);
	CCSize levelDigitSize = levelDigitSprite->getContentSize();
	levelDigitSprite->setPosition(ccp(goLevelSize.width / 2,goLevelSize.height / 2));
	goLevelSize = goLevelSize * scale;
	scale =  goLevelSize.height * 0.8f / levelDigitSize.height;
	levelDigitSprite->setScale(scale);
	goLevelSprite->addChild(levelDigitSprite);

	CCActionInterval *moveTo = CCMoveTo::create(1.0f, ccp(winSize.width / 2,winSize.height / 2));
	CCActionInterval *easeBounceOut = CCEaseBounceOut ::create(moveTo);
	CCActionInterval *delayTime = CCDelayTime::create(1.0f);
	CCActionInterval *moveTo2 = CCMoveTo::create(0.5f,ccp(winSize.width / 2,winSize.height + goLevelSize.height));
	CCActionInterval *easeBounceIn = CCEaseBounceIn ::create(moveTo2);
	CCCallFunc *funCall = CCCallFunc::create(this, callfunc_selector(LinkCard::goLevelStartCallBack));
	CCFiniteTimeAction *seq = CCSequence::create(easeBounceOut, delayTime, easeBounceIn, funCall, NULL);
	goLevelSprite->runAction(seq);

	AudioPlayer::shared()->playEffect("sounds/effect_ready_go.mp3");
}

void LinkCard::goLevelStartCallBack(void)
{
}

void LinkCard::goLevelEnd(bool isPass)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite *goLevelSprite;

	m_lock = true;
	this->unschedule(schedule_selector(LinkCard::scoreLevelHandler));
	this->unschedule(schedule_selector(LinkCard::timeUpdateCallBack));
	BackGround::shared()->setRedColor(false);

	if(isPass)
	{
		if(m_level == UserData::shared()->getLevel())
			UserData::shared()->setLevel(m_level + 1);
        
        int getstart = UserData::shared()->getStarLevel(m_level);
        
        
		int starLevel = 0;
		int time = m_timeDownMax;
		if(m_timeDown > (int)(time * LINK_STAR_LEVLE_3))
			starLevel = 2;
		else if(m_timeDown > (int)(time * LINK_STAR_LEVLE_2))
			starLevel = 1;
		UserData::shared()->setStarLevel(m_level, starLevel);
        
        if(starLevel > getstart)
        {
            int curnum = UserData::shared()->getStartNum();
            UserData::shared()->setStartNum(curnum+starLevel+1);
        }
        
	}

	if(isPass)
	{
		//AudioPlayer::shared()->playEffect("sounds/effect_timeout.mp3");
		goLevelSprite = CCSprite::create("images/game_go_level_pass.png");
	}
	else
	{
		AudioPlayer::shared()->playEffect("sounds/effect_timeout.mp3");
		goLevelSprite = CCSprite::create("images/game_go_level_timeout.png");
	}
	CCSize goLevelSize = goLevelSprite->getContentSize();
	float scale = winSize.width / goLevelSize.width;
	goLevelSprite->setScale(scale);
	goLevelSprite->setPosition(ccp(winSize.width / 2,winSize.height / 2));
	goLevelSprite->setOpacity(0);
	this->addChild(goLevelSprite, 201 + LINKCARD_ZORDER_MAX);

	CCActionInterval *delayTime = CCDelayTime::create(1.0f);
	CCActionInterval *fadeIn = CCFadeIn::create(0.5f);
	CCActionInterval *fadeOut = CCFadeOut::create(0.5f);
	CCCallFunc *funCall;
	if(isPass)
		funCall = CCCallFunc::create(this, callfunc_selector(LinkCard::goLevelEndPassCallBack));
	else
		funCall = CCCallFunc::create(this, callfunc_selector(LinkCard::goLevelEndFailCallBack));
	CCFiniteTimeAction *seq = CCSequence::create(fadeIn, delayTime, fadeOut, funCall, NULL);
	goLevelSprite->runAction(seq);
}

void LinkCard::goLevelEndPassCallBack(void)
{
	gamePass();
}

void LinkCard::goLevelEndFailCallBack(void)
{
	gameOver();
}

void LinkCard::scoreLevelHandler(float dt)
{
	if(m_scoreCount < LINKCARD_SCORE_COUNT_MAX + 1)
		m_scoreCount++;
	if(m_scoreCount == LINKCARD_SCORE_COUNT_MAX)
	{
		if(m_scoreLevel != 0)
		{
			m_scoreLevel = 0;
			GameInfo::shared()->setScoreLevel(m_scoreLevel);
		}
	}
}

void LinkCard::gameOver(void)
{
	AudioPlayer::shared()->playEffect("sounds/effect_lose.mp3");
	BackGround::shared()->setRedColor(false);
	this->addChild(DialogTimeout::create(), 200 + LINKCARD_ZORDER_MAX, LinkCardTagGameTimeout);
}

void LinkCard::gamePass(void)
{
	int starLevel = UserData::shared()->getStarLevel(m_level);
	AudioPlayer::shared()->playEffect("sounds/effect_win.mp3");
	DialogGameEnd *gameEndDialog = DialogGameEnd::create(DialogGameEndModePass, m_level, starLevel, m_timeCount);
	this->addChild(gameEndDialog, 200 + LINKCARD_ZORDER_MAX, LinkCardTagGameEnd);
}

void LinkCard::showFailDialog(void)
{
	DialogGameEnd *gameEndDialog = DialogGameEnd::create(DialogGameEndModeFail, m_level, 0, 0);
	this->addChild(gameEndDialog, 200 + LINKCARD_ZORDER_MAX, LinkCardTagGameEnd);
}

int LinkCard::getMaxTime()
{
	return m_timeDownMax;
}

void LinkCard::setMaxTime(int msec)
{
	if(msec < 0)
		msec = 0;
	m_timeDownMax = msec;
}

int LinkCard::getTime()
{
	return m_timeDown;
}

void LinkCard::setTime(int msec)
{
	if(msec < 1)
		m_timeDown = 1;
	else if(msec > m_timeDownMax)
		m_timeDown = m_timeDownMax;
	else
		m_timeDown = msec;
	GameInfo::shared()->setTime(m_timeDown);
}

void LinkCard::clearLinkCardCallBack(float dt)
{
	if(m_clearLock == 0 || m_clearLink == NULL || m_clearLink2 == NULL)
		return;

	//m_linkSprite[m_clearLink->y * m_xCards + m_clearLink->x] = NULL;
	//m_linkSprite[m_clearLink2->y * m_xCards + m_clearLink2->x] = NULL;

	CCParticleSystemQuad *particle = CCParticleSystemQuad::create("particles/stars2.plist");
	particle->setAutoRemoveOnFinish(true);
	particle->setBlendAdditive(true);
	particle->setPosition(m_clearLink->getPosition());
	this->addChild(particle,100 + LINKCARD_ZORDER_MAX);

	CCParticleSystemQuad *particle2 = CCParticleSystemQuad::create("particles/stars2.plist");
	particle2->setAutoRemoveOnFinish(true);
	particle2->setBlendAdditive(true);
	particle2->setPosition(m_clearLink2->getPosition());
	this->addChild(particle2,100 + LINKCARD_ZORDER_MAX);

	clearFreezeCard(m_clearLink);
	clearFreezeCard(m_clearLink2);
	removeChild(m_clearLink);
	removeChild(m_clearLink2);

	m_lightning->stopLighting();

	m_clearLink = NULL;
	m_clearLink2 = NULL;

	if(m_count != 0)
	{
		if(!addNewCard())
			if(!checkLink())
				refreshCard(true);
	}
	else
	{
		goLevelEnd(true);
	}

	m_clearLock = false;
}

bool LinkCard::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	if(m_lock || m_count == 0)
		return false;

	if(m_clearLock)
	{
		this->unschedule(schedule_selector(LinkCard::clearLinkCardCallBack));
		clearLinkCardCallBack(0);
	}

	int x,y;
	CCPoint point = pTouch->getLocationInView();
	point = CCDirector::sharedDirector()->convertToGL(point);

	if(!getXY(point, &x, &y))
		return false;

	CardSprite *m_linkSel2 = m_linkSprite[y * m_xCards + x];
	if(m_linkSel2 == NULL)
		return true;

	if(m_linkSel2->getDisable())
		return true;

	if(m_linkHelp1 != NULL)
	{
		m_linkHelp1->setBlink(false);
		m_linkHelp1 = NULL;
	}
	if(m_linkHelp2 != NULL)
	{
		m_linkHelp2->setBlink(false);
		m_linkHelp2 = NULL;
	}

	if(m_linkSel2 == m_linkSel)
		return true;
	else
		m_linkSel2->setSelected(true);

	if(m_linkSel == NULL)
	{
		m_linkSel = m_linkSel2;
		AudioPlayer::shared()->playEffect("sounds/effect_choose.mp3");
	}
	else
	{
		if(isLink(m_linkSel->x,m_linkSel->y,m_linkSel2->x,m_linkSel2->y,true))
		{
			for(int index = 0;index < m_linkLineCount;index++)
			{
				m_lightning->setLighting(index,getPos(m_linkLine[index].x1,m_linkLine[index].y1),
					getPos(m_linkLine[index].x2,m_linkLine[index].y2));
			}
			if(m_linkLineCount != 0)
				m_lightning->drawLighting(0);
			m_linkLineCount = 0;

			m_clearLock = true;
			m_clearLink = m_linkSel;
			m_clearLink2 = m_linkSel2;

			m_linkSprite[m_linkSel->y * m_xCards + m_linkSel->x] = NULL;
			m_linkSprite[m_linkSel2->y * m_xCards + m_linkSel2->x] = NULL;

			this->scheduleOnce(schedule_selector(LinkCard::clearLinkCardCallBack), 0.4f);

			m_count -= 2;
			m_linkSel = NULL;
			m_linkSel2 = NULL;

			if(m_scoreCount < LINKCARD_SCORE_COUNT_MAX / 4)
				AudioPlayer::shared()->playEffect("sounds/effect_disappear_5.mp3");
			else if(m_scoreCount < LINKCARD_SCORE_COUNT_MAX)
				AudioPlayer::shared()->playEffect("sounds/effect_disappear_3.mp3");
			else
				AudioPlayer::shared()->playEffect("sounds/effect_disappear_1.mp3");

			if(m_scoreCount < LINKCARD_SCORE_COUNT_MAX)
			{
				if(m_scoreCount < LINKCARD_SCORE_COUNT_MAX / 4)
					m_scoreLevel += 2;
				else
					m_scoreLevel += 1;
				if(m_scoreLevel % GAMEINFO_SCORE_LEVEL_SIZE == 0)
				{
					int time;
					int level = m_scoreLevel / GAMEINFO_SCORE_LEVEL_SIZE;
					if(level == 1)
						time  = m_timeDownMax * 0.1f;
					else if(level == 2)
						time  = m_timeDownMax * 0.15f;
					else if(level == 3)
						time  = m_timeDownMax * 0.2f;
					else
						time  = m_timeDownMax * 0.25f;
					setTime(getTime() + time);
				}
				if(m_scoreLevel >= GAMEINFO_SCORE_LEVEL_SIZE * GAMEINFO_SCORE_LEVEL_MAX)
					m_scoreLevel = 0;
				GameInfo::shared()->setScoreLevel(m_scoreLevel);
			}
			m_scoreCount = 0;
		}
		else
		{
			AudioPlayer::shared()->playEffect("sounds/effect_choose.mp3");
		 	m_linkSel->setSelected(false);
			m_linkSel = m_linkSel2;
			if(m_scoreLevel != 0)
			{
				m_scoreLevel = 0;
				GameInfo::shared()->setScoreLevel(m_scoreLevel);
			}
		}
	}

	return true;
}

void LinkCard::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority + 1, true);
}

void LinkCard::keyBackClicked(void)
{
	CCLayer *gameTimeoutDialog = (CCLayer *)this->getChildByTag(LinkCardTagGameTimeout);
	CCLayer *gameEndDialog = (CCLayer *)this->getChildByTag(LinkCardTagGameEnd);
	CCLayer *buyGiftDialog = (CCLayer *)this->getChildByTag(LinkCardTagBuyGift);

	if(buyGiftDialog != NULL)
	{
		buyGiftDialog->keyBackClicked();
		buyGiftDialog = NULL;
	}
	else if(gameEndDialog != NULL)
	{
		gameEndDialog->keyBackClicked();
		gameEndDialog = NULL;
	}
	else if(gameTimeoutDialog != NULL)
	{
		gameTimeoutDialog->keyBackClicked();
		gameTimeoutDialog = NULL;
	}
	else
	{
		pauseGame();
	}
}

void LinkCard::clearFreezeCard(CardSprite *cardSprite)
{
	CardSprite *tempCardSprite;

	if(cardSprite->x > 0)
	{
		tempCardSprite = m_linkSprite[cardSprite->y * m_xCards + (cardSprite->x - 1)];
		if(tempCardSprite != NULL && tempCardSprite->getFlags(CardSpriteFlagFreeze))
			tempCardSprite->setFreeze(false);
	}
	if(cardSprite->x < m_xCards - 1)
	{
		tempCardSprite = m_linkSprite[cardSprite->y * m_xCards + (cardSprite->x + 1)];
		if(tempCardSprite != NULL && tempCardSprite->getFlags(CardSpriteFlagFreeze))
			tempCardSprite->setFreeze(false);
	}
	if(cardSprite->y > 0)
	{
		tempCardSprite = m_linkSprite[(cardSprite->y - 1) * m_xCards + cardSprite->x];
		if(tempCardSprite != NULL && tempCardSprite->getFlags(CardSpriteFlagFreeze))
			tempCardSprite->setFreeze(false);
	}
	if(cardSprite->y < m_yCards - 1)
	{
		tempCardSprite = m_linkSprite[(cardSprite->y + 1) * m_xCards + cardSprite->x];
		if(tempCardSprite != NULL && tempCardSprite->getFlags(CardSpriteFlagFreeze))
			tempCardSprite->setFreeze(false);
	}
}

bool LinkCard::checkNormalCard(CardSprite *cardSprite)
{
	bool result = false;

	if(cardSprite->x > 0)
		if(m_linkSprite[cardSprite->y * m_xCards + (cardSprite->x - 1)] != NULL)
			result = true;

	if(cardSprite->x < m_xCards - 1)
		if(m_linkSprite[cardSprite->y * m_xCards + (cardSprite->x + 1)] != NULL)
			result = true;

	if(cardSprite->y > 0)
		if(m_linkSprite[(cardSprite->y - 1) * m_xCards + cardSprite->x] != NULL)
			result = true;

	if(cardSprite->y < m_yCards - 1)
		if(m_linkSprite[(cardSprite->y + 1) * m_xCards + cardSprite->x] != NULL)
			result = true;

	return result;
}

bool LinkCard::addNewCard(void)
{
	int index1,index2;

	int total = m_cardMap->getTotal();
	if(m_addCount >= total)
		return false;

	int cardCount = m_cardMap->getCardCount() / 2;
	if(cardCount % 2 != 0)
		cardCount--;

	if(m_count > cardCount)
		return false;

	//存储生成的新卡片
	//int addCount = cardCount / 2;
	int addCount = cardCount;
	if(addCount % 2 != 0)
		addCount--;
	if(total - m_addCount < addCount)
		addCount = total - m_addCount;
	CardSprite **cardSpriteTable = new CardSprite *[addCount];

	//暂存可以放置卡片的位置
	int remCount = m_cardMap->getCardCount() - m_count;
	int *cardIndexTable = new int[remCount];

	//生成新的卡片
	int temp = addCount / 2;
	for(index1 = 0; index1 < temp; index1++)
	{
		int id = rand() % CardSprite::getCardTotal(m_cardStyle) + 1;
		cardSpriteTable[index1] = CardSprite::create(m_cardStyle, id);
	}
	for(index1 = 0; index1 < temp; index1++)
	{
		cardSpriteTable[index1 + temp] = CardSprite::create(m_cardStyle,cardSpriteTable[index1]->getID());
	}

	//将所有可以放置卡片位置提取出来
	for(index1 = 0, index2 = 0; index1 < m_xCards * m_yCards; index1++)
	{
		if(m_linkSprite[index1] == NULL)
		{
			int cardType = m_cardMap->getCardType(index1);
			if(cardType == LinkMapTypeNormal || cardType == LinkMapTypeFreeze)
				cardIndexTable[index2++] = index1;
		}
	}

	//打乱位置
	for(index1 = 0; index1 < remCount; index1++)
	{
		int i = rand() % remCount;
		index2 = cardIndexTable[index1];
		cardIndexTable[index1] = cardIndexTable[i];
		cardIndexTable[i] = index2;
	}

	//将新卡片添加到m_linkSprite中
	for(index1 = 0, index2 = 0; index1 < addCount; index1++, index2++)
	{
		cardSpriteTable[index1]->x = cardIndexTable[index2] % m_xCards;
		cardSpriteTable[index1]->y = cardIndexTable[index2] / m_xCards;
		m_linkSprite[cardIndexTable[index2]] = cardSpriteTable[index1];

		cardSpriteTable[index1]->setScale(m_scale * LINKCARD_SCALE);
		cardSpriteTable[index1]->setPosition(getPos(cardSpriteTable[index1]->x, cardSpriteTable[index1]->y));
		this->addChild(cardSpriteTable[index1], cardIndexTable[index2] + LINKCARD_ZORDER_BASE);
		cardSpriteTable[index1]->setFadeIn();
	}

	//更新冰封状态
	for(index1 = 0; index1 < index2; index1++)
	{
		if(m_cardMap->getCardType(cardIndexTable[index1]) == LinkMapTypeFreeze)
		{
			if(m_linkSprite[cardIndexTable[index1]] != NULL)
			{
				if(checkNormalCard(m_linkSprite[cardIndexTable[index1]]))
				{
					m_linkSprite[cardIndexTable[index1]]->setFreeze(true);
				}
			}
		}
	}

	delete[] cardIndexTable;
	delete[] cardSpriteTable;

	m_addCount += addCount;
	m_count += addCount;

	if(!checkLink())
	{
		refreshCard(true);
	}

	return true;
}

const CCPoint LinkCard::getPos(int x,int y)
{
	return CCPoint(m_xStart + m_width * x + m_width / 2,m_yStart - m_height * y - m_height / 2);
}

bool LinkCard::getXY(const CCPoint &pos, int *x, int *y)
{
	float x1 = (pos.x - m_xStart) / m_width;
	float y1 = (m_yStart - pos.y) / m_height;

	if(x1 < 0 || x1 > m_xCards || y1 < 0 || y1 > m_yCards)
		return false;

	*x = (int)x1;
	*y = (int)y1;

	return true;
}

int LinkCard::getID(int x,int y)
{
	if(m_linkSprite[y * m_xCards + x] == NULL)
		return LINKCARD_INVALID_ID;

	if(m_linkSprite[y * m_xCards + x]->getFlags(CardSpriteFlagFreeze))
		return LINKCARD_FREEZE_STATUS;

	int style = m_linkSprite[y * m_xCards + x]->getStyle();
	int id = m_linkSprite[y * m_xCards + x]->getID();
	if(style >= CardSpriteStyleStart && style <= CardSpriteStyleEnd)
		return id;
	return LINKCARD_DISABLE_STATUS;
}

int LinkCard::getIDEx(int x,int y)
{
	x -= 1;
	y -= 1;
	if(x < 0 || x >= m_xCards || y < 0 || y >= m_yCards)
		return LINKCARD_INVALID_ID;

	if(m_linkSprite[y * m_xCards + x] == NULL)
		return LINKCARD_INVALID_ID;

	if(m_linkSprite[y * m_xCards + x]->getFlags(CardSpriteFlagFreeze))
		return LINKCARD_FREEZE_STATUS;

	int style = m_linkSprite[y * m_xCards + x]->getStyle();
	int id = m_linkSprite[y * m_xCards + x]->getID();
	if(style >= CardSpriteStyleStart && style <= CardSpriteStyleEnd)
		return id;
	return LINKCARD_DISABLE_STATUS;
}

void LinkCard::setLinkLine(int index,int x1,int y1,int x2,int y2)
{
	if(index < LinkCardLinkLineMax)
	{
		m_linkLine[index].x1 = x1 - 1;
		m_linkLine[index].y1 = y1 - 1;
		m_linkLine[index].x2 = x2 - 1;
		m_linkLine[index].y2 = y2 - 1;
		m_linkLineCount = index + 1;
	}
}

bool LinkCard::isAdj(int x1,int y1,int x2,int y2,bool updatePos)
{
	if(x1 == x2)
	{
		if(abs(y1 - y2) == 1)
		{
			if(updatePos)
			{
				setLinkLine(0, x1, y1, x2, y2);
			}
			return true;
		}
	}
	else if(y1 == y2)
	{
		if(abs(x1 - x2) == 1)
		{
			if(updatePos)
			{
				setLinkLine(0, x1, y1, x2, y2);
			}
			return true;
		}
	}
	return false;
}

/*
start=true:x1,y1位置不检查
end=true:x2,y2位置不检查
*/
bool LinkCard::isLine(int x1,int y1,int x2,int y2,bool start,bool end,bool updatePos)
{
	int temp;
	int tx1 = x1;
	int ty1 = y1;
	int tx2 = x2;
	int ty2 = y2;
	if(x1 == x2 && y1 == y2)
	{
		if(updatePos)
		{
			setLinkLine(0, tx1, ty1, tx2, ty2);
		}
		return true;
	}
	if(x1 == x2)
	{
		if(y1 > y2)
		{
			if(start)
				y1--;
			if(end)
				y2++;
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		else
		{
			if(start)
				y1++;
			if(end)
				y2--;
		}
		while(y1 <= y2)
		{
			if(getIDEx(x1,y1) != LINKCARD_INVALID_ID)
				return false;
			y1++;
		}
		if(updatePos)
		{
			setLinkLine(0, tx1, ty1, tx2, ty2);
		}

		return true;
	}
	else if(y1 == y2)
	{
		if(x1 > x2)
		{
			if(start)
				x1--;
			if(end)
				x2++;
			temp = x1;
			x1 = x2;
			x2 = temp;
		}
		else
		{
			if(start)
				x1++;
			if(end)
				x2--;
		}
		while(x1 <= x2)
		{
			if(getIDEx(x1,y1) != LINKCARD_INVALID_ID)
				return false;
			x1++;
		}
		if(updatePos)
		{
			setLinkLine(0, tx1, ty1, tx2, ty2);
		}
		return true;
	}
	return false;
}


bool LinkCard::isLineToLine(int x1,int y1,int x2,int y2,bool updatePos)
{
	if(isLine(x1,y1,x2,y1,true,false) && isLine(x2,y1,x2,y2,true,true))
	{
		if(updatePos)
		{
			setLinkLine(0, x1, y1, x2, y1);
			setLinkLine(1, x2, y1, x2, y2);
		}
		return true;
	}
	if(isLine(x1,y1,x1,y2,true,false) && isLine(x1,y2,x2,y2,true,true))
	{
		if(updatePos)
		{
			setLinkLine(0, x1, y1, x1, y2);
			setLinkLine(1, x1, y2, x2, y2);
		}
		return true;
	}
	return false;
}

bool LinkCard::isLineToLineToLine(int x1,int y1,int x2,int y2,bool updatePos)
{
	int line;

	for(line = x1;line < LINKCARD_XLINK_MAX;line++)
	{
		if(isLine(line,y1,x1,y1,false,true) &&
			isLine(line,y2,x2,y2,false,true) &&
			isLine(line,y1,line,y2,true,true))
		{
			if(updatePos)
			{
				setLinkLine(0, x1, y1, line, y1);
				setLinkLine(1, line, y1, line, y2);
				setLinkLine(2, line, y2, x2, y2);
			}
			return true;
		}
	}
	for(line = x1 - 1;line >= 0;line--)
	{
		if(isLine(line,y1,x1,y1,false,true) &&
			isLine(line,y2,x2,y2,false,true) &&
			isLine(line,y1,line,y2,true,true))
		{
			if(updatePos)
			{
				setLinkLine(0, x1, y1, line, y1);
				setLinkLine(1, line, y1, line, y2);
				setLinkLine(2, line, y2, x2, y2);
			}
			return true;
		}
	}

	for(line = y1;line < LINKCARD_YLINK_MAX;line++)
	{
		if(isLine(x1,line,x1,y1,false,true) &&
			isLine(x2,line,x2,y2,false,true) &&
			isLine(x1,line,x2,line,true,true))
		{
			if(updatePos)
			{
				setLinkLine(0, x1, y1, x1, line);
				setLinkLine(1, x1, line, x2, line);
				setLinkLine(2, x2, line, x2, y2);
			}
			return true;
		}
	}
	for(line = y1 - 1;line >= 0;line--)
	{
		if(isLine(x1,line,x1,y1,false,true) &&
			isLine(x2,line,x2,y2,false,true) &&
			isLine(x1,line,x2,line,true,true))
		{
			if(updatePos)
			{
				setLinkLine(0, x1, y1, x1, line);
				setLinkLine(1, x1, line, x2, line);
				setLinkLine(2, x2, line, x2, y2);
			}
			return true;
		}
	}
	return false;
}

bool LinkCard::isLink(int x1,int y1,int x2,int y2,bool updatePos)
{
	if(x1 == x2 && y1 == y2)
		return false;
	x1++;
	y1++;
	x2++;
	y2++;
	if(getIDEx(x2,y2) < 0)
		return false;
	if(getIDEx(x1,y1) != getIDEx(x2,y2))
		return false;
	if(isAdj(x1,y1,x2,y2,updatePos))
		return true;
	if(isLine(x1,y1,x2,y2,true,true,updatePos))
		return true;
	if(isLineToLine(x1,y1,x2,y2,updatePos))
		return true;
	if(isLineToLineToLine(x1,y1,x2,y2,updatePos))
		return true;
	return false;
}


bool LinkCard::searchLink(int *x1,int *y1,int *x2,int *y2)
{
	int _x1,_y1,_x2,_y2;
	for(_y1 = 0;_y1 < m_yCards;_y1++)
	{
		for(_x1 = 0;_x1 < m_xCards;_x1++)
		{
			if(getID(_x1,_y1) <= LINKCARD_INVALID_ID)
				continue;
			for(_y2 = 0;_y2 < m_yCards;_y2++)
			{
				for(_x2 = 0;_x2 < m_xCards;_x2++)
				{
					if(_x1 == _x2 && _y1 == _y2)
						continue;
					if(getID(_x1,_y1) <= LINKCARD_INVALID_ID)
						continue;
					if(isLink(_x1,_y1,_x2,_y2))
					{
						if(x1 != NULL)
							*x1 = _x1;
						if(x2 != NULL)
							*x2 = _x2;
						if(y1 != NULL)
							*y1 = _y1;
						if(y2 != NULL)
							*y2 = _y2;
						return true;
					}
				}
			}
		}
	}
	return false;
}


bool LinkCard::checkLink(void)
{
	return searchLink(NULL,NULL,NULL,NULL);
}

void LinkCard::clearSelSprite(void)
{
	if(m_linkSel != NULL)
	{
		m_linkSel->setSelected(false);
		m_linkSel = NULL;
	}
	if(m_linkHelp1 != NULL)
	{
		m_linkHelp1->setBlink(false);
		m_linkHelp1 = NULL;
	}
	if(m_linkHelp2 != NULL)
	{
		m_linkHelp2->setBlink(false);
		m_linkHelp2 = NULL;
	}
}

void LinkCard::refreshCardCallBack(float dt)
{
	m_lock = false;
}

bool LinkCard::refreshCard(bool action)
{
	int index1,index2;

	if(m_lock || m_count == 0)
		return false;
	m_lock = true;

	clearSelSprite();

	int count = m_cardMap->getCardCount();
	int size = m_cardMap->getSize();
	int *cardIndexTable = new int[count];
	CardSprite **cardSpriteTable = new CardSprite *[m_count];

	do
	{
		//将所有可以放置卡片位置提取出来
		for(index1 = 0, index2 = 0; index1 < size; index1++)
		{
			int cardType = m_cardMap->getCardType(index1);
			if(cardType == LinkMapTypeNormal || cardType == LinkMapTypeFreeze)
				cardIndexTable[index2++] = index1;
		}

		//提取当前可消除状态的卡片
		for(index1 = 0, index2 = 0;index1 < size;index1++)
		{
			if(m_linkSprite[index1] != NULL)
			{
				int style = m_linkSprite[index1]->getStyle();
				if(style >= CardSpriteStyleStart && style <= CardSpriteStyleEnd)
				{
					if(m_linkSprite[index1]->getFlags(CardSpriteFlagFreeze))
						m_linkSprite[index1]->setFreeze(false);
					cardSpriteTable[index2++] = m_linkSprite[index1];
					m_linkSprite[index1] = NULL;
				}
			}
		}

		//打乱位置
		for(index1 = 0; index1 < count; index1++)
		{
			int i = rand() % count;
			index2 = cardIndexTable[index1];
			cardIndexTable[index1] = cardIndexTable[i];
			cardIndexTable[i] = index2;
		}

		//将未清除的卡片添加到m_linkSprite中
		for(index1 = 0, index2 = 0; index1 < m_count; index1++, index2++)
		{
			cardSpriteTable[index1]->setZOrder(cardIndexTable[index2] + LINKCARD_ZORDER_BASE);
			cardSpriteTable[index1]->x = cardIndexTable[index2] % m_xCards;
			cardSpriteTable[index1]->y = cardIndexTable[index2] / m_xCards;
			m_linkSprite[cardIndexTable[index2]] = cardSpriteTable[index1];
			if(!action)
				cardSpriteTable[index1]->setPosition(getPos(cardSpriteTable[index1]->x, cardSpriteTable[index1]->y));
		}

		//更新冰封状态
		for(index1 = 0; index1 < m_xCards * m_yCards; index1++)
		{
			int cardType = m_cardMap->getCardType(index1);
			if(cardType == LinkMapTypeFreeze && m_linkSprite[index1] != NULL)
				if(checkNormalCard(m_linkSprite[index1]))
					m_linkSprite[index1]->setFreeze(true);
		}
	}while(!checkLink());

	delete[] cardIndexTable;
	delete[] cardSpriteTable;

	if(action)
	{
		for(int y = 0;y < m_yCards;y++)
		{
			for(int x = 0;x < m_xCards;x++)
			{
				CardSprite *cardSprite = m_linkSprite[y * m_xCards + x];
				if(cardSprite != NULL)
				{
					CCActionInterval *move = CCMoveTo::create(0.5,getPos(x, y));
					cardSprite->stopAllActions();
					cardSprite->runAction(move);
				}
			}
		}
		AudioPlayer::shared()->playEffect("sounds/effect_refresh.mp3");
		this->scheduleOnce(schedule_selector(LinkCard::refreshCardCallBack), 0.5f);
	}

	return true;
}

void LinkCard::bombCardCallBack(float dt)
{
	AudioPlayer::shared()->playEffect("sounds/effect_bomb.mp3");
	for(int i = 0; i < m_xCards * m_yCards; i++)
	{
		if(m_linkSprite[i] != NULL && m_linkSprite[i]->getFlags(CardSpriteFlagAim))
		{
			CCParticleSystemQuad* particle= CCParticleSystemQuad::create("particles/debris.plist");
			particle->setAutoRemoveOnFinish(true);
			particle->setBlendAdditive(true);
			particle->setPosition(m_linkSprite[i]->getPosition());
			this->addChild(particle,100 + LINKCARD_ZORDER_MAX);

			clearFreezeCard(m_linkSprite[i]);
			removeChild(m_linkSprite[i]);

			//m_count -= 1;
			m_linkSprite[i] = NULL;
		}
	}

	if(m_count != 0)
	{
		if(!addNewCard())
			if(!checkLink())
				refreshCard(true);
	}
	else
	{
		//PlayGame::shared()->nextLevel();
		goLevelEnd(true);
	}

	m_lock = false;
}

bool LinkCard::bombCard(void)
{
	if(m_lock || m_count == 0)
		return false;
	m_lock = true;

	CardSprite *cardSprite = m_linkSel;
	int index = rand() % m_count;

	clearSelSprite();

	if(cardSprite == NULL)
	{
		for(int i = 0; i < m_xCards * m_yCards; i++)
		{
			if(m_linkSprite[i] != NULL)
			{
				int style = m_linkSprite[i]->getStyle();
				if(style >= CardSpriteStyleStart && style <= CardSpriteStyleEnd)
				{
					if(index-- == 0)
					{
						cardSprite = m_linkSprite[i];
						break;
					}
				}
			}
		}
	}

	for(int i = 0; i < m_xCards * m_yCards; i++)
	{
		if(m_linkSprite[i] != NULL)
		{
			if( m_linkSprite[i]->getStyle() == cardSprite->getStyle()
				&& m_linkSprite[i]->getID() == cardSprite->getID())
			{
				m_linkSprite[i]->setAim(true);
				m_count -= 1;
			}
		}
	}

	AudioPlayer::shared()->playEffect("sounds/effect_bomb_aim.mp3");
	this->scheduleOnce(schedule_selector(LinkCard::bombCardCallBack), 0.3f);

	return true;
}

bool LinkCard::helpCard(void)
{
	int x1,y1,x2,y2;

	if(m_lock || m_count == 0)
		return false;
	m_lock = true;

	if(searchLink(&x1,&y1,&x2,&y2))
	{
		clearSelSprite();

		m_linkHelp1 = m_linkSprite[m_xCards * y1 + x1];
		m_linkHelp2 = m_linkSprite[m_xCards * y2 + x2];

		CCAssert(m_linkHelp1 != NULL, "m_linkHelp1 Error");
		CCAssert(m_linkHelp2 != NULL, "m_linkHelp2 Error");

		AudioPlayer::shared()->playEffect("sounds/effect_help.mp3");
		m_linkHelp1->setBlink(true);
		m_linkHelp2->setBlink(true);
	}

	m_lock = false;

	return true;
}

bool LinkCard::addTime(void)
{
	if(m_lock || m_count == 0)
		return false;
	m_lock = true;

	AudioPlayer::shared()->playEffect("sounds/effect_time.mp3");
	setTime(getTime() + m_timeDownMax * 0.3f);

	m_lock = false;

	return true;
}

int LinkCard::refreshProp(void)
{
	if(UserData::shared()->getToolNum(REFRESHTOOL) > 0)
	{
		if(!refreshCard(true))
			return LinkCardPropError;
        int curtool = UserData::shared()->getToolNum(REFRESHTOOL);
        curtool--;
        UserData::shared()->setToolNum(REFRESHTOOL, curtool);
        GameMenu::shared()->setToolNum(REFRESHTOOL, UserData::shared()->getToolNum(REFRESHTOOL));
	}
	else
	{
		gameStore(KTAGBUYTOOL);
	}

	return LinkCardPropSucceed;
}

int LinkCard::bombProp(void)
{
	if(UserData::shared()->getToolNum(BOMBTOOL) > 0)
	{
		if(!bombCard())
			return LinkCardPropError;
        int curtool = UserData::shared()->getToolNum(BOMBTOOL);
        curtool--;
        UserData::shared()->setToolNum(BOMBTOOL, curtool);
        GameMenu::shared()->setToolNum(BOMBTOOL, UserData::shared()->getToolNum(BOMBTOOL));
	}
	else
	{
		gameStore(KTAGBUYTOOL);
	}

	return LinkCardPropSucceed;
}

int LinkCard::helpProp(void)
{
	if(UserData::shared()->getToolNum(FOUNDTOOL) > 0)
	{
		if(!helpCard())
			return LinkCardPropError;
        int curtool = UserData::shared()->getToolNum(FOUNDTOOL);
        curtool--;
        UserData::shared()->setToolNum(FOUNDTOOL, curtool);
        GameMenu::shared()->setToolNum(FOUNDTOOL, UserData::shared()->getToolNum(FOUNDTOOL));
	}
	else
	{
		gameStore(KTAGBUYTOOL);
	}

	return LinkCardPropSucceed;
}


int LinkCard::timeProp(void)
{
	if(UserData::shared()->getToolNum(TIMETOOL) > 0)
	{
		if(!addTime())
			return LinkCardPropError;
        int curtool = UserData::shared()->getToolNum(TIMETOOL);
        curtool--;
        UserData::shared()->setToolNum(TIMETOOL, curtool);
        GameMenu::shared()->setToolNum(TIMETOOL, UserData::shared()->getToolNum(TIMETOOL));
	}
	else
	{
		gameStore(KTAGBUYTOOL);
	}

	return LinkCardPropSucceed;
}

int LinkCard::nextLevelProp(void)
{
	int result = LinkCardPropNotEnough;
	bool lock = m_lock;
	m_lock = false;

	if((readGameVirgour() >= LinkCardPropPriceNext)
		|| (m_level < UserData::shared()->getLevel()))
	{
		if(nextGame() != LinkCardPropSucceed)
		{
			m_lock = lock;
			return LinkCardPropError;
		}
		if(m_level == UserData::shared()->getLevel())
		{
			UserData::shared()->setLevel(m_level + 1);
			UserData::shared()->setStarLevel(m_level, 2);
            saveGameVirgour(readGameVirgour()-LinkCardPropPriceNext);
			//UserData::shared()->setGoldCoins(UserData::shared()->getGoldCoins() - LinkCardPropPriceNext);
			//GameInfo::shared()->setCoin(UserData::shared()->getGoldCoins());
            
		}
		result = LinkCardPropSucceed;
	}
	else
	{
		gameStore(KTAGBUYGOLD);
	}
	m_lock = lock;

	return result;
}

int LinkCard::doubleTimeProp(void)
{
	int result = LinkCardPropNotEnough;
	bool lock = m_lock;
	m_lock = false;

	if(readGameVirgour() >= LinkCardPropPriceDoubleTime)
	{
		if(restartGameWithDoubleTime() != LinkCardPropSucceed)
		{
			m_lock = lock;
			return LinkCardPropError;
		}
        saveGameVirgour(readGameVirgour()-LinkCardPropPriceDoubleTime);
		//UserData::shared()->setGoldCoins(UserData::shared()->getGoldCoins() - LinkCardPropPriceDoubleTime);
		//GameInfo::shared()->setCoin(UserData::shared()->getGoldCoins());
		result = LinkCardPropSucceed;
	}
	else
	{
		gameStore(KTAGBUYGOLD);
	}
	m_lock = lock;

	return result;
}


int LinkCard::gameStore(int kshop,bool gift)
{
	if(m_lock)
		return LinkCardPropError;

	for(int i = 0; i < m_xCards * m_yCards; i++)
	{
		if(m_linkSprite[i] != NULL)
		{
			int style = m_linkSprite[i]->getStyle();
			if(style >= CardSpriteStyleStart && style <= CardSpriteStyleEnd)
				m_linkSprite[i]->setHide(true, 0);
		}
	}

    AudioPlayer::shared()->shared();
	this->unschedule(schedule_selector(LinkCard::scoreLevelHandler));
	this->unschedule(schedule_selector(LinkCard::timeUpdateCallBack));
	//this->addChild(DialogBuyGift::create(kshop,gift), 400 + LINKCARD_ZORDER_MAX, LinkCardTagBuyGift);

	return LinkCardPropSucceed;
}

int LinkCard::backGameWithStore(void)
{
	for(int i = 0; i < m_xCards * m_yCards; i++)
	{
		if(m_linkSprite[i] != NULL)
		{
			int style = m_linkSprite[i]->getStyle();
			if(style >= CardSpriteStyleStart && style <= CardSpriteStyleEnd)
				m_linkSprite[i]->setHide(false, 0);
		}
	}

	if(m_timeDown != 0)
	{
		this->schedule(schedule_selector(LinkCard::scoreLevelHandler), 0.1f);
		this->schedule(schedule_selector(LinkCard::timeUpdateCallBack), 0.01f);
	}

	return LinkCardPropSucceed;
}

int LinkCard::pauseGame(void)
{
	if(m_lock)
		return LinkCardPropError;

	for(int i = 0; i < m_xCards * m_yCards; i++)
	{
		if(m_linkSprite[i] != NULL)
		{
			int style = m_linkSprite[i]->getStyle();
			if(style >= CardSpriteStyleStart && style <= CardSpriteStyleEnd)
				m_linkSprite[i]->setHide(true, 0);
		}
	}

	CCDirector::sharedDirector()->pushScene(DialogPause::scene());
	return LinkCardPropSucceed;
}

int LinkCard::resumeGame(void)
{
	for(int i = 0; i < m_xCards * m_yCards; i++)
	{
		if(m_linkSprite[i] != NULL)
		{
			int style = m_linkSprite[i]->getStyle();
			if(style >= CardSpriteStyleStart && style <= CardSpriteStyleEnd)
				m_linkSprite[i]->setHide(false, 0);
		}
	}

	return LinkCardPropSucceed;
}

int LinkCard::nextGame(void)
{
	this->unschedule(schedule_selector(LinkCard::timeUpdateCallBack));
	PlayGame::shared()->setLevel(m_level + 1, LinkCardInitModeDefault);
	return LinkCardPropSucceed;
}

int LinkCard::restartGame(void)
{
	this->unschedule(schedule_selector(LinkCard::timeUpdateCallBack));
	PlayGame::shared()->setLevel(m_level, LinkCardInitModeDefault);
	return LinkCardPropSucceed;
}

int LinkCard::restartGameWithDoubleTime(void)
{
	this->unschedule(schedule_selector(LinkCard::timeUpdateCallBack));
	PlayGame::shared()->setLevel(m_level, LinkCardInitModeDoubleTime);
	return LinkCardPropSucceed;
}
