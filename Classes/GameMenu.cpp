

#include "AppConfig.h"
#include "GameMenu.h"
#include "LinkCard.h"


USING_NS_CC;

#define GAMEMENU_ITEM_WIDTH			(160 + 40)
#define GAMEMENU_ITEM_HEIGHT		(160)
#define GAMEMENU_X_MARGIN			(0.01f)
#define GAMEMENU_Y_MARGIN			(0.01f)
#define GAMEMENU_Y_START			SCREEN_LINE1_START
#define GAMEMENU_Y_HEIGHT			SCREEN_LINE1_HEIGHT
#define GAMEMENU_X_START			SCREEN_GAMEMENU_START
#define GAMEMENU_X_WIDTH			SCREEN_GAMEMENU_WIDTH

#define GAMEMENU_GROUP_SPACE		(0.2f)

static GameMenu *sharedGameMenu = NULL;

GameMenu *GameMenu::shared()
{
    //	CCAssert(sharedGameInfo != NULL,"sharedGameInfo Invalid!!");
    return sharedGameMenu;
}

GameMenu::GameMenu()
{
    sharedGameMenu = this;
}

GameMenu::~GameMenu()
{
    sharedGameMenu = NULL;
}

bool GameMenu::initWithMenu()
{
	float width,height,wScale,hScale;
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	width = winSize.width * (GAMEMENU_X_WIDTH - GAMEMENU_GROUP_SPACE - GAMEMENU_X_MARGIN * 2);
	height = winSize.height * (GAMEMENU_Y_HEIGHT - GAMEMENU_Y_MARGIN * 2);
	float itemWidth = width / GameMenuItemMax;
	wScale = itemWidth / GAMEMENU_ITEM_WIDTH;
	hScale = height / GAMEMENU_ITEM_HEIGHT;
	if(wScale < hScale)
		m_scale = wScale;
	else
		m_scale = hScale;
	m_width = GAMEMENU_ITEM_WIDTH * m_scale;
	m_height = GAMEMENU_ITEM_HEIGHT * m_scale;

	m_xStart = winSize.width * GAMEMENU_X_START + itemWidth / 2 + 
		winSize.width * GAMEMENU_X_MARGIN;
	m_yStart = winSize.height * GAMEMENU_Y_START + height / 2 + 
		winSize.height * GAMEMENU_Y_MARGIN;

	m_selSprite = CCSprite::create("images/prop_click.png");
	m_selSprite->setScale(m_scale);
	m_selSprite->setOpacity(0);
	addChild(m_selSprite,0);

	m_menuItem[GameMenuItemPause] = CCMenuItemImage::create(
		"images/play_button_pause.png",
		NULL,
		this,
		menu_selector(GameMenu::pauseMenuCallback));
	m_menuItem[GameMenuItemPause]->setScale(m_scale);
	m_menuItem[GameMenuItemPause]->setPosition(ccp(m_xStart + itemWidth * GameMenuItemPause,m_yStart));
	m_xStart += winSize.width * GAMEMENU_GROUP_SPACE;

	m_menuItem[GameMenuItemRefresh] = CCMenuItemImage::create(
		"images/play_button_refresh.png",
		NULL,
		this,
		menu_selector(GameMenu::refreshMenuCallback));
	m_menuItem[GameMenuItemRefresh]->setScale(m_scale);
	m_menuItem[GameMenuItemRefresh]->setPosition(ccp(m_xStart + itemWidth * GameMenuItemRefresh,m_yStart));
    
    //刷新道具
    m_toolsnumSprite[REFRESHTOOL] = DigitSprite::create(0);
    m_toolsnumSprite[REFRESHTOOL]->output(UserData::shared()->getToolNum(REFRESHTOOL));
    m_toolsnumSprite[REFRESHTOOL]->setPosition(ccp(m_menuItem[GameMenuItemRefresh]->getContentSize().width*0.2,m_menuItem[GameMenuItemRefresh]->getContentSize().height*0.2));
    m_menuItem[GameMenuItemRefresh]->addChild(m_toolsnumSprite[REFRESHTOOL]);

	m_menuItem[GameMenuItemHelp] = CCMenuItemImage::create(
		"images/play_button_help.png",
		NULL,
		this,
		menu_selector(GameMenu::helpMenuCallback));
	m_menuItem[GameMenuItemHelp]->setScale(m_scale);
	m_menuItem[GameMenuItemHelp]->setPosition(ccp(m_xStart + itemWidth * GameMenuItemHelp,m_yStart));
    
    //帮助道具
    m_toolsnumSprite[FOUNDTOOL] = DigitSprite::create(0);
    m_toolsnumSprite[FOUNDTOOL]->output(UserData::shared()->getToolNum(FOUNDTOOL));
    m_toolsnumSprite[FOUNDTOOL]->setPosition(ccp(m_menuItem[GameMenuItemHelp]->getContentSize().width*0.2,m_menuItem[GameMenuItemHelp]->getContentSize().height*0.2));
    m_menuItem[GameMenuItemHelp]->addChild(m_toolsnumSprite[FOUNDTOOL]);

	m_menuItem[GameMenuItemTime] = CCMenuItemImage::create(
		"images/play_button_time.png",
		NULL,
		this,
		menu_selector(GameMenu::timeMenuCallback));
	m_menuItem[GameMenuItemTime]->setScale(m_scale);
	m_menuItem[GameMenuItemTime]->setPosition(ccp(m_xStart + itemWidth * GameMenuItemTime,m_yStart));
    
    //时间道具
    m_toolsnumSprite[TIMETOOL] = DigitSprite::create(0);
    m_toolsnumSprite[TIMETOOL]->output(UserData::shared()->getToolNum(TIMETOOL));
    m_toolsnumSprite[TIMETOOL]->setPosition(ccp(m_menuItem[GameMenuItemTime]->getContentSize().width*0.2,m_menuItem[GameMenuItemTime]->getContentSize().height*0.2));
    m_menuItem[GameMenuItemTime]->addChild(m_toolsnumSprite[TIMETOOL]);

	m_menuItem[GameMenuItemBomb] = CCMenuItemImage::create(
		"images/play_button_bomb.png",
		NULL,
		this,
		menu_selector(GameMenu::bombMenuCallback));
	m_menuItem[GameMenuItemBomb]->setScale(m_scale);
	m_menuItem[GameMenuItemBomb]->setPosition(ccp(m_xStart + itemWidth * GameMenuItemBomb,m_yStart));
    
    //炸弹道具
    m_toolsnumSprite[BOMBTOOL] = DigitSprite::create(0);
    m_toolsnumSprite[BOMBTOOL]->output(UserData::shared()->getToolNum(BOMBTOOL));
    m_toolsnumSprite[BOMBTOOL]->setPosition(ccp(m_menuItem[GameMenuItemBomb]->getContentSize().width*0.2,m_menuItem[GameMenuItemBomb]->getContentSize().height*0.2));
    m_menuItem[GameMenuItemBomb]->addChild(m_toolsnumSprite[BOMBTOOL]);

	CCMenu *menu = CCMenu::create(m_menuItem[GameMenuItemPause], NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu, 1);

	return true;
}

bool GameMenu::init()
{
	if(!CCLayer::init())
        return false;
	if(!initWithMenu())
        return false;
	return true;
}

GameMenu *GameMenu::create()
{
    GameMenu *pRet = new GameMenu();
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

void GameMenu::setToolNum(int ktool, int num)
{
    m_toolsnumSprite[ktool]->output(num);
}

void GameMenu::menuClickAction(CCObject* pSender)
{
	//m_selSprite->setOpacity(255);
	m_selSprite->setScale(m_scale);
	m_selSprite->setPosition(((CCMenuItemImage *)pSender)->getPosition());
	CCActionInterval *fadeOut = CCFadeOut::create(0.5f);
	CCActionInterval *scaleTo = CCScaleTo ::create(0.5f, m_scale * 1.3f);
	CCFiniteTimeAction *spawn = CCSpawn::create(fadeOut, scaleTo, NULL);
	m_selSprite->runAction(spawn);
}

void GameMenu::pauseMenuCallback(CCObject* pSender)
{
	int result = LinkCard::shared()->pauseGame();
	if(result != LinkCardPropError)
		menuClickAction(pSender);
}

void GameMenu::refreshMenuCallback(CCObject* pSender)
{
	int result = LinkCard::shared()->refreshProp();
	if(result != LinkCardPropError)
		menuClickAction(pSender);
}

void GameMenu::helpMenuCallback(CCObject* pSender)
{
	int result = LinkCard::shared()->helpProp();
	if(result != LinkCardPropError)
		menuClickAction(pSender);
}

void GameMenu::timeMenuCallback(CCObject* pSender)
{
	int result = LinkCard::shared()->timeProp();
	if(result != LinkCardPropError)
		menuClickAction(pSender);
}

void GameMenu::bombMenuCallback(CCObject* pSender)
{
	int result = LinkCard::shared()->bombProp();
	if(result != LinkCardPropError)
		menuClickAction(pSender);
}

