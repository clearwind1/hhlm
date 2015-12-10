#include "LevelMap.h"
#include "PlayGame.h"
#include "UserData.h"
#include "MainMenu.h"
#include "LabelTextLine.h"
#include "DialogSetting.h"
#include "DialogLogonGift.h"
#include "DialogSuperGift.h"
//#include "DialogBuyGift.h"
#include "Funtion.h"
#include "VirgourLayer.h"

USING_NS_CC;

#define LEVELMAP_IMAGE_WIDTH		(512)
#define LEVELMAP_IMAGE_HEIGHT		(512)

#define LEVELMAP_LOCK_SCALE			(0.7f)

#define LEVELMAP_BUTTON_X_MARGIN	(0.05f)
#define LEVELMAP_BUTTON_Y_MARGIN	(0.03f)

#define LEVELMAP_BUTTON_X_SPACE		(0.05f)
#define LEVELMAP_BUTTON_Y_SPACE		(0.05f)

#define LEVELMAP_SETTING_DIALOG_TAG		(100)
#define LEVELMAP_LOGONGIFT_DIALOG_TAG	(101)
#define LEVELMAP_SUPERGIFT_DIALOG_TAG	(102)


enum
{
	LevelMapTagMin = 100,
	LevelMapTagSetting,
	LevelMapTagLogonGift,
	LevelMapTagSuperGift,
	LevelMapTagMax
};

static const char *mapFileName[LEVEL_MAP_MAX] = 
{
	"images/map_1.jpg",
	"images/map_2.jpg",
	"images/map_3.jpg",
	"images/map_4.jpg",
	"images/map_5.jpg",
	"images/map_6.jpg",
	"images/map_7.jpg",
	"images/map_8.jpg",
	"images/map_9.jpg",
	"images/map_10.jpg",
	"images/map_11.jpg"
};

static const char *starLevelFileName[3] = 
{
	"images/map_star_1.png",
	"images/map_star_2.png",
	"images/map_star_3.png"
};

struct LockPos
{
	int x;
	int y;
};

#if 0
static const struct LockPos lockPos[LEVEL_MAP_LOCK_MAX] = 
{
{189, 298},
{292, 310},
{371, 352},
{313, 431},
{211, 443},
{114, 464},
{49, 523},
{112, 589},
{205, 610},
{324, 628},
{347, 719},
{244, 756},
{134, 767},
{55, 830},
{116, 920},
{246, 935},
{344, 946},
{342, 1039},
{256, 1080},
{149, 1086},
{60, 1133},
{82, 1223},
{185, 1243},
{293, 1246},
{351, 1334},
{267, 1397},
{149, 1401},
{60, 1472},
{121, 1551},
{228, 1558},
{339, 1586},
{319, 1691},
{218, 1710},
{108, 1726},
{59, 1807},
{129, 1870},
{231, 1885},
{336, 1896},
{311, 1963},
{234, 2106},
{133, 2135},
{57, 2187},
{116, 2260},
{212, 2276},
{316, 2295},
{348, 2384},
{261, 2438},
{155, 2444},
{56, 2470},
{77, 2562},
{173, 2595},
{277, 2603},
{355, 2677},
{278, 2749},
{161, 2754},
{62, 2789},
{78, 2886},
{184, 2914},
{282, 2921},
{347, 2991},
{280, 3065},
{176, 3072},
{75, 3082},
{79, 3172},
{181, 3183},
{284, 3193},
{360, 3249},
{310, 3327},
{216, 3338},
{118, 3356},
{40, 3412},
{99, 3486},
{188, 3501},
{288, 3510},
{348, 3575},
{283, 3642},
{178, 3657},
{74, 3682},
{45, 3768},
{128, 3820},
{231, 3828},
{333, 3843},
{335, 3940},
{236, 3978},
{123, 3985},
{41, 4049}
};
#endif
static const struct LockPos lockPos[LEVEL_MAP_LOCK_MAX] = 
{
{244, 380},
{351, 393},
{456, 426},
{469, 519},
{364, 559},
{259, 570},
{149, 588},
{73, 651},
{124, 745},
{248, 781},
{377, 797},
{461, 857},
{416, 946},
{301, 967},
{185, 981},
{84, 1031},
{77, 1127},
{170, 1183},
{316, 1193},
{433, 1208},
{446, 1317},
{335, 1380},
{204, 1388},
{87, 1432},
{97, 1563},
{190, 1583},
{303, 1591},
{410, 1604},
{456, 1698},
{388, 1776},
{289, 1794},
{161, 1800},
{74, 1892},
{139, 1977},
{259, 1991},
{374, 1997},
{453, 2075},
{399, 2164},
{273, 2187},
{149, 2209},
{75, 2288},
{135, 2384},
{254, 2410},
{383, 2415},
{462, 2444},
{397, 2525},
{305, 2696},
{198, 2728},
{88, 2767},
{88, 2866},
{195, 2901},
{308, 2913},
{414, 2937},
{452, 3036},
{369, 3115},
{252, 3126},
{143, 3130},
{61, 3186},
{96, 3278},
{206, 3324},
{316, 3326},
{419, 3355},
{445, 3461},
{347, 3517},
{229, 3521},
{119, 3540},
{66, 3621},
{120, 3709},
{231, 3732},
{335, 3734},
{427, 3780},
{430, 3879},
{333, 3931},
{218, 3929},
{115, 3936},
{69, 4014},
{152, 4073},
{264, 4077},
{370, 4088},
{455, 4151},
{418, 4253},
{298, 4271},
{175, 4286},
{68, 4334},
{83, 4445},
{216, 4480},
{349, 4494},
{446, 4565},
{374, 4653},
{239, 4681},
{112, 4703},
{59, 4814},
{176, 4892},
{322, 4898},
{438, 4925},
{430, 5039},
{303, 5086},
{165, 5099},
{55, 5174}
};

static LevelMap *sharedLevelMap = NULL;

LevelMap *LevelMap::shared()
{
    //	CCAssert(sharedGameInfo != NULL,"sharedGameInfo Invalid!!");
    return sharedLevelMap;
}

LevelMap::LevelMap()
{
    sharedLevelMap = this;
}
LevelMap::~LevelMap()
{
    sharedLevelMap = NULL;
}


CCScene* LevelMap::scene()
{
    CCScene *scene = CCScene::create();

    LevelMap *layer = LevelMap::create();

    scene->addChild(layer);

    return scene;
}

LevelMap *LevelMap::create()
{
    LevelMap *pRet = new LevelMap();
    if(pRet && pRet->init()) 
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool LevelMap::init()
{
    if(!CCLayer::init())
        return false;
    
    if(!UserData::shared()->getFirstRun())
    {
        UserData::shared()->setGoldCoins(5);
        UserData::shared()->setFirstRun(true);
        
        saveGameVirgour(5);
    }
    
    this->addChild(VirgourLayer::create(acWinsizeWidth*0.5-190, 50),50);

	for(int index = 0; index < LEVEL_MAP_MAX; index++)
	{
		m_map[index] = CCSprite::create((const char *)mapFileName[index]);
		this->addChild(m_map[index]);
	}

	m_winSize = CCDirector::sharedDirector()->getWinSize();
	m_imgSize = m_map[0]->getContentSize();
	m_scale = m_winSize.width / m_imgSize.width;
	m_imgSize = m_imgSize * m_scale;
	m_mapSize.width = m_imgSize.width;
	m_mapSize.height = m_imgSize.height * LEVEL_MAP_MAX;

	for(int index = 0; index < LEVEL_MAP_MAX; index++)
	{
		m_map[index]->setScale(m_scale);
		m_map[index]->setAnchorPoint(ccp(0.0f, 0.0f));
		m_map[index]->setPosition(ccp(0, m_imgSize.height * index));
	}

	m_curLevel = UserData::shared()->getLevel();

	m_lockInfoSpriteBatchNode = CCSpriteBatchNode::create("images/map_lock_info1.png");
	m_lockInfoSpriteBatchNode->setPosition(ccp(0,0));
	this->addChild(m_lockInfoSpriteBatchNode);

	CCRect unlockRect = CCRectMake(0, 0, 60, 70);
	CCRect starLevelRect[3];
	starLevelRect[0] = CCRectMake(60, 0, 60, 20);
	starLevelRect[1] = CCRectMake(60, 20, 60, 20);
	starLevelRect[2] = CCRectMake(60, 40, 60, 20);
	CCRect lockRect = CCRectMake(120, 0, 60, 70);
	CCRect curlockRect = CCRectMake(180, 0, 60, 70);
	CCRect levelTextRect = CCRectMake(0, 70, 200, 20);

	LabelTextLine *labelTextLine = LabelTextLine::create(m_lockInfoSpriteBatchNode->getTexture(), levelTextRect, 20, 20, '0');
	this->addChild(labelTextLine);

	for(int index = 0; index < m_curLevel - 1; index++)
	{
		CCSprite *unlockSprite  = CCSprite::createWithTexture(m_lockInfoSpriteBatchNode->getTexture(), unlockRect);
		unlockSprite->setScale(m_scale * LEVELMAP_LOCK_SCALE);
		unlockSprite->setPosition(ccp(lockPos[index].x * m_scale, lockPos[index].y * m_scale));
		m_lockInfoSpriteBatchNode->addChild(unlockSprite);

		CCSprite *starLevelSprite = CCSprite::createWithTexture(m_lockInfoSpriteBatchNode->getTexture(), starLevelRect[UserData::shared()->getStarLevel(index + 1)]);
		CCAssert(starLevelSprite != NULL,"ERROR!");
		//starLevelSprite->setAnchorPoint(ccp(0, 0));
		CCSize starLevelSpriteSize = starLevelSprite->getContentSize();
		starLevelSprite->setPosition(ccp(starLevelSpriteSize.width * 0.5f, starLevelSpriteSize.height * 0.5f));
		unlockSprite->addChild(starLevelSprite);

		CCSprite *levelText = labelTextLine->textWithFormat("%d", index + 1);
		CCSize unlockSpriteSize = unlockSprite->getContentSize();
		levelText->setPosition(ccp(unlockSpriteSize.width * 0.5f, unlockSpriteSize.height * 0.7f));
		unlockSprite->addChild(levelText);
		//labelTextLine->textWithFormat(unlockSprite, "%d", index + 1);

		m_lock[index] = unlockSprite;
	}

	if(m_curLevel < LEVEL_MAP_LOCK_MAX)
	{
		m_lock[m_curLevel - 1] = CCSprite::createWithTexture(m_lockInfoSpriteBatchNode->getTexture(), curlockRect);
		CCSprite *curSprite = m_lock[m_curLevel - 1];
		m_lock[m_curLevel - 1]->setScale(m_scale * LEVELMAP_LOCK_SCALE);
		m_lock[m_curLevel - 1]->setPosition(ccp(lockPos[m_curLevel - 1].x * m_scale, lockPos[m_curLevel - 1].y * m_scale));
		m_lockInfoSpriteBatchNode->addChild(m_lock[m_curLevel - 1]);
		CCActionInterval *scaleTo = CCScaleTo ::create(0.4f, curSprite->getScale() * 1.2f);
		CCActionInterval *scaleTo2 = CCScaleTo ::create(0.9f, curSprite->getScale());
		CCFiniteTimeAction *seq = CCSequence::create(scaleTo, scaleTo2, NULL);
		CCActionInterval *repeatForever = CCRepeatForever::create((CCActionInterval* )seq);
		curSprite->runAction(repeatForever);

		m_keySprite = CCSprite::create("images/map_lock_key.png");
		m_keySprite->setScale(m_scale * LEVELMAP_LOCK_SCALE);
		m_keySprite->setPosition(ccp(lockPos[m_curLevel - 1].x * m_scale, lockPos[m_curLevel - 1].y * m_scale));
		this->addChild(m_keySprite);
		CCActionInterval *rotateTo = CCRotateTo::create(1.0f, 720);
		CCFiniteTimeAction *seq2 = CCSequence::create(rotateTo, NULL);
		CCActionInterval *repeatForever2 = CCRepeatForever::create((CCActionInterval* )seq2);
		m_keySprite->runAction(repeatForever2);

		for(int index = m_curLevel; index < LEVEL_MAP_LOCK_MAX; index++)
		{
			CCSprite *lockSprite  = CCSprite::createWithTexture(m_lockInfoSpriteBatchNode->getTexture(), lockRect);
			lockSprite->setScale(m_scale * LEVELMAP_LOCK_SCALE);
			lockSprite->setPosition(ccp(lockPos[index].x * m_scale, lockPos[index].y * m_scale));
			m_lockInfoSpriteBatchNode->addChild(lockSprite);
			m_lock[index] = lockSprite;
		}
	}

	//菜单项
	CCMenuItemImage *backMenuItem = CCMenuItemImage::create(
		"images/dialog_button_back.png",
		NULL,
		this,
		menu_selector(LevelMap::backMenuCallback));
	CCSize backMenuSize = backMenuItem->getContentSize();
	CCMenuItemImage *settingMenuItem = CCMenuItemImage::create(
		"images/button_setting.png",
		NULL,
		this,
		menu_selector(LevelMap::settingMenuCallback));
	CCSize settingMenuSize = settingMenuItem->getContentSize();

	//float scale = m_winSize.width * (1 - LEVELMAP_BUTTON_X_MARGIN * 2 - LEVELMAP_BUTTON_X_SPACE) / (backMenuSize.width + startMenuSize.width);
	float scale = (m_winSize.width * 0.15f) / backMenuSize.width;

	float xStart = m_winSize.width * LEVELMAP_BUTTON_X_MARGIN;
	float yStart = m_winSize.height * LEVELMAP_BUTTON_Y_MARGIN;

	backMenuItem->setScale(scale);
	backMenuSize = backMenuSize * scale; 
	backMenuItem->setPosition(ccp(xStart + backMenuSize.width * 0.5f, yStart + backMenuSize.height * 0.5f));
	yStart += backMenuSize.height;// + m_winSize.height * LEVELMAP_BUTTON_Y_SPACE;

	settingMenuItem->setScale(scale);
	settingMenuSize = settingMenuSize * scale;
	settingMenuItem->setPosition(ccp(xStart + settingMenuSize.width * 0.5f, yStart + settingMenuSize.height * 0.5f));
	yStart += settingMenuSize.height;// + m_winSize.height * LEVELMAP_BUTTON_Y_SPACE;
    
//    CCMenuItemImage* shopbtn = createMenu("images/shopbtn.png", "images/shopbtn.png", this, menu_selector(LevelMap::openShopMenuCallback), 608, 1151);
//    shopbtn->setTag(KTAGBUYTOOL);
    
    CCMenuItemImage* virgourbtn = createMenu("images/game_info_coin_bg.png", "images/game_info_coin_bg.png", this, menu_selector(LevelMap::openShopMenuCallback), 360, 54);
    virgourbtn->setTag(KTAGBUYGOLD);
    m_virgoutnumSprite = DigitSprite::create(0);
    m_virgoutnumSprite->setAnchorPoint(ccp(0,0.5));
    m_virgoutnumSprite->output(UserData::shared()->getGoldCoins());
    m_virgoutnumSprite->setPosition(ccp(75,43));
    virgourbtn->addChild(m_virgoutnumSprite);
    m_virgoutnumSprite->setScale(0.7);

	CCMenu *menu = CCMenu::create(backMenuItem, settingMenuItem, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu, 1);

	m_pos = 0;
	m_moveY = 0;
	m_startFlag = false;

	//当前关卡居中显示
	CCPoint centerPos = m_lock[m_curLevel - 1]->getPosition();
	setPos(centerPos.y - m_winSize.height * 0.5f);
    
    //this->scheduleOnce(schedule_selector(LevelMap::superGiftCallBack), 0.05f);
    
    //logonGiftStar();
    
    /*
    CCSprite* startFrame = createSprite("images/startFrame.png", 186, 58);
    this->addChild(startFrame,10);
    m_startnumSprite = DigitSprite::create(0);
    m_startnumSprite->setAnchorPoint(ccp(0,0.5));
    m_startnumSprite->output(UserData::shared()->getStartNum());
    m_startnumSprite->setPosition(ccp(75,46));
    startFrame->addChild(m_startnumSprite);
    m_startnumSprite->setScale(0.7);
     */
    
	this->setTouchEnabled(true);
	this->setKeypadEnabled(true);
    
    return true;
}

void LevelMap::openShopMenuCallback(cocos2d::CCObject *pSender)
{
    int tag = ((CCMenuItemImage*)pSender)->getTag();
    
    //this->addChild(DialogBuyGift::create(tag),100);
    
}

void LevelMap::setnumSprite()
{
    //m_startnumSprite->output(UserData::shared()->getStartNum());
    m_virgoutnumSprite->output(UserData::shared()->getGoldCoins());
}

void LevelMap::backMenuCallback(CCObject* pSender)
{
	CCScene *scene = MainMenu::scene(true);
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(0.5f,scene));
}

void LevelMap::settingMenuCallback(CCObject* pSender)
{
	this->addChild(DialogSetting::create(), 100, LevelMapTagSetting);
}

void LevelMap::setPos(float pos)
{
	float tmp = m_pos + pos;

	if(tmp < 0)
		tmp = 0;
	if(tmp > m_mapSize.height - m_winSize.height)
		tmp = m_mapSize.height - m_winSize.height;

	for(int index = 0; index < LEVEL_MAP_MAX; index++)
	{
		m_map[index]->setPosition(ccp(0, m_imgSize.height * index - tmp));
	}
	CCPoint position = m_keySprite->getPosition();
	position.y -= tmp - m_pos;
	m_keySprite->setPosition(position);
	for(int index = 0; index < LEVEL_MAP_LOCK_MAX; index++)
	{
		CCPoint position = m_lock[index]->getPosition();
		position.y -= tmp - m_pos;
		m_lock[index]->setPosition(position);
	}
	m_pos = tmp;
}

void LevelMap::superGiftCallBack(float dt)
{
    srand(time(NULL));
    if(!UserData::shared()->getFirstRun())
        this->addChild(DialogSuperGift::create(), 100, LevelMapTagSuperGift);
    else
    {
        int rr = rand()%2;
        if(rr == 0)
            ;//this->addChild(DialogBuyGift::create(0,true),100, LevelMapTagSuperGift);
        else
            this->addChild(DialogSuperGift::create(),100, LevelMapTagSuperGift);
    }
    
    
//	if(UserData::shared()->getGoldCoins() < 880)
//	{
//		
//	}
//	else
//	{
//		logonGiftStar();
//	}
    
   // logonGiftStar();
}

void LevelMap::logonGiftStar()
{
	if(UserData::shared()->getGiftCount() != UserData::shared()->getLogonCount())
	{
		this->addChild(DialogLogonGift::create(), 100, LevelMapTagLogonGift);
	}
}

void LevelMap::timeUpdateCallBack(float dt)
{
	m_touchCount++;
}

void LevelMap::startMovePos(float dt)
{
	if(m_endPos != 0)
	{
		if(fabs(m_endPos) > fabs(m_step))
		{
			setPos(m_step);
			m_endPos -= m_step;
			m_step *= 0.92f;
			if(fabs(m_step) < 1.0f)
			{
				if(m_step < 0)
					m_step = -1.0f;
				else
					m_step = 1.0f;
			}
		}
		else
		{
			setPos(m_endPos);
			m_endPos = 0;
		}
	}
	else
	{
		this->unschedule(schedule_selector(LevelMap::startMovePos));
	}
}

void LevelMap::startLevel(int level)
{
	CCLog("level,%d", level);
    subGameVirgour();
	m_keySprite->stopAllActions();
	CCScene *pScene = PlayGame::scene(level, false);
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(0.5f, pScene));
}

bool LevelMap::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint point = pTouch->getLocationInView();
	point = CCDirector::sharedDirector()->convertToGL(point);

	m_selLevel = -1;
	this->unschedule(schedule_selector(LevelMap::startMovePos));
	for(int index = 0; index < LEVEL_MAP_LOCK_MAX; index++)
	{
		CCSprite *sprite = (CCSprite *)m_lock[index];
		CCSize halfSize = sprite->getContentSize() * sprite->getScale() * 0.5f;
		CCPoint position = sprite->getPosition();
		if(point.x >= position.x - halfSize.width
			&& point.x <= position.x + halfSize.width
			&& point .y >= position.y - halfSize.height
			&& point .y <= position.y + halfSize.height)
		{
			m_selLevel = index;
			break;
		}
	}

	m_MovePosIndex = 0;
	for(int i = 0; i < LEVEL_MAP_MOVEPOS_MAX; i++)
		m_MovePos[i] = 0;

	//if(m_selLevel < 0)
	{
		m_beganY = point.y;
		m_moveY = m_beganY;
		//m_touchCount = 0;
		//this->schedule(schedule_selector(LevelMap::timeUpdateCallBack), 0.01f);
	}

	return true;
}

void LevelMap::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint point = pTouch->getLocationInView();
	point = CCDirector::sharedDirector()->convertToGL(point);

	if(m_selLevel < 0)
	{
		m_MovePos[m_MovePosIndex++] = m_moveY - point.y;
		if(m_MovePosIndex >= LEVEL_MAP_MOVEPOS_MAX)
			m_MovePosIndex = 0;
		setPos(m_moveY - point.y);
		m_moveY = point.y;
	}
	else if(fabs(m_moveY - point.y) > 5.0f)
	{
		m_MovePos[m_MovePosIndex++] = m_moveY - point.y;
		if(m_MovePosIndex >= LEVEL_MAP_MOVEPOS_MAX)
			m_MovePosIndex = 0;		setPos(m_moveY - point.y);
		m_moveY = point.y;
		m_selLevel = -1;
	}
}

void LevelMap::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint point = pTouch->getLocationInView();
	point = CCDirector::sharedDirector()->convertToGL(point);

	if(m_selLevel < 0)
	{
		float pos = 0;
		for(int i = 0; i < LEVEL_MAP_MOVEPOS_MAX; i++)
		{
			if(fabs(pos) < fabs(m_MovePos[i]))
				pos = m_MovePos[i];
		}

		CCLog("MovePos:%f", pos);
		if(fabs(pos) > 10)
		{
			m_endPos = pos * 10;
			m_step = pos;
			this->schedule(schedule_selector(LevelMap::startMovePos), 0.01f);
		}
		/*
		this->unschedule(schedule_selector(LevelMap::timeUpdateCallBack));

		float ySize = (m_beganY - point.y) * (float)m_touchCount;
		if(fabs(ySize) < 10)
			return;
		m_endPos = ySize;
		m_step = fabs(ySize) / (float)m_touchCount;
		startMovePos(0);*/
	}
	else
	{
		for(int index = 0; index < LEVEL_MAP_LOCK_MAX; index++)
		{
			CCSprite *sprite = (CCSprite *)m_lock[index];
			CCSize halfSize = sprite->getContentSize() * sprite->getScale() * 0.5f;
			CCPoint position = sprite->getPosition();
			if(point.x >= position.x - halfSize.width
				&& point.x <= position.x + halfSize.width
				&& point .y >= position.y - halfSize.height
				&& point .y <= position.y + halfSize.height)
			{
				if(m_selLevel == index && m_selLevel < m_curLevel)
				{
					startLevel(m_selLevel + 1);
					break;
				}
			}
		}
	}
}

void LevelMap::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority + 1, true);
}

void LevelMap::keyBackClicked(void)
{
	CCLayer *settingDialog = (CCLayer *)this->getChildByTag(LevelMapTagSetting);
	CCLayer *logonGiftDialog = (CCLayer *)this->getChildByTag(LevelMapTagLogonGift);
	CCLayer *superGiftDialog = (CCLayer *)this->getChildByTag(LevelMapTagSuperGift);

	if(settingDialog != NULL)
	{
		settingDialog->keyBackClicked();
		settingDialog = NULL;
	}
	else if(superGiftDialog != NULL)
	{
		superGiftDialog->keyBackClicked();
		superGiftDialog = NULL;
	}
	else if(logonGiftDialog != NULL)
	{
		logonGiftDialog->keyBackClicked();
		logonGiftDialog = NULL;
	}
	else
	{
		backMenuCallback(NULL);
	}
}
