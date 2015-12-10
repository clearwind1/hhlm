

#include "OptionMenu.h"
#include "MainMenu.h"
#include "AudioPlayer.h"

USING_NS_CC;

enum
{
	MENU_LANG_EN = 0,
	MENU_LANG_CN,
	MENU_LANG_MAX
};


enum
{
	MENU_ITEM_MUSIC = 0,
	MENU_ITEM_EFFECT,
	MENU_ITEM_MAINMENU,
	MENU_ITEM_MAX
};


enum
{
	MENU_STA_NORMAL = 0,
	MENU_STA_SELECTED,
	MENU_STA_DISABLED,
	MENU_STA_MAX
};


#define SET_MENU_IMG_WIDTH			(480)
#define SET_MENU_IMG_HEIGHT			(160)
#define SET_MENU_MARGIN_SIZE		(0.2f)

#define MENU_LANG_DEFAULT			MENU_LANG_CN


static const char *menuImgName[MENU_LANG_MAX][MENU_ITEM_MAX][MENU_STA_MAX] =
{
	{
		{"menu_music_on.png","menu_music_off.png",NULL},
		{"menu_effect_on.png","menu_effect_off.png",NULL},
		{"menu_main_menu.png","menu_main_menu.png",NULL}
	},
	{
		{"menu_music_on_cn.png","menu_music_off_cn.png",NULL},
		{"menu_effect_on_cn.png","menu_effect_off_cn.png",NULL},
		{"menu_main_menu_cn.png","menu_main_menu_cn.png",NULL}
	}
};

extern CCSprite *LoadBgImage(const char *filename);


CCScene *OptionMenu::scene()
{
    CCScene *scene = CCScene::create();

	OptionMenu *layer = OptionMenu::create();
	scene->addChild(layer);

	return scene;
}

OptionMenu *OptionMenu::create()
{
    OptionMenu *pRet = new OptionMenu();
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

bool OptionMenu::init()
{
    if(!CCLayer::init())
        return false;

	this->addChild(LoadBgImage("images/bg_06.png"),0);

	initMenu();

	this->setTouchEnabled(true);
    
    return true;
}



void OptionMenu::initMenu()
{
	float width,height;
	float menuWidth,menuHeight;
	float menuXStart,menuYStart;
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	width = winSize.width - winSize.width * (SET_MENU_MARGIN_SIZE * 2);
	if(SET_MENU_IMG_WIDTH <= width)
	{
		menuWidth = SET_MENU_IMG_WIDTH;
		menuHeight = SET_MENU_IMG_HEIGHT;
		m_menuScale = 1;
	}
	else
	{
		menuWidth = width;
		m_menuScale = menuWidth / SET_MENU_IMG_WIDTH;
		menuHeight = SET_MENU_IMG_HEIGHT * m_menuScale;
	}
	height = menuHeight * MENU_ITEM_MAX;

	menuXStart = winSize.width / 2;
	menuYStart = winSize.height - (winSize.height - menuHeight * MENU_ITEM_MAX) / 2 - menuHeight / 2;

	int m_lang = 0;
	CCMenuItemImage *musicMenuItem = CCMenuItemImage::create(
		menuImgName[m_lang][MENU_ITEM_MUSIC][MENU_STA_NORMAL],
		menuImgName[m_lang][MENU_ITEM_MUSIC][MENU_STA_SELECTED],
		this,
		menu_selector(OptionMenu::musicMenuCallback));
	musicMenuItem->setScale(m_menuScale);
	musicMenuItem->setPosition(ccp(menuXStart,menuYStart));
	menuYStart -= menuHeight;

	CCMenuItemImage *effectMenuItem = CCMenuItemImage::create(
		menuImgName[m_lang][MENU_ITEM_EFFECT][MENU_STA_NORMAL],
		menuImgName[m_lang][MENU_ITEM_EFFECT][MENU_STA_SELECTED],
		this,
		menu_selector(OptionMenu::effectMenuCallback));
	effectMenuItem->setScale(m_menuScale);
	effectMenuItem->setPosition(ccp(menuXStart,menuYStart));
	menuYStart -= menuHeight;

	CCMenuItemImage *mainMenuItem = CCMenuItemImage::create(
		menuImgName[m_lang][MENU_ITEM_MAINMENU][MENU_STA_NORMAL],
		menuImgName[m_lang][MENU_ITEM_MAINMENU][MENU_STA_SELECTED],
		this,
		menu_selector(OptionMenu::mainMenuCallback));
	mainMenuItem->setScale(m_menuScale);
	mainMenuItem->setPosition(ccp(menuXStart,menuYStart));
	menuYStart -= menuHeight;

	CCMenu *menu = CCMenu::create(musicMenuItem,effectMenuItem,mainMenuItem, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu, 1);
}


void OptionMenu::musicMenuCallback(CCObject* pSender)
{
	bool mute = AudioPlayer::shared()->isMusicMute();
	if(mute == false)
		((CCMenuItemImage *)pSender)->unselected();
	else
		((CCMenuItemImage *)pSender)->selected();
	AudioPlayer::shared()->setMusicMute(!mute);
}


void OptionMenu::effectMenuCallback(CCObject* pSender)
{
	bool mute = AudioPlayer::shared()->isEffectMute();
	if(mute == false)
		((CCMenuItemImage *)pSender)->unselected();
	else
		((CCMenuItemImage *)pSender)->selected();
	AudioPlayer::shared()->setEffectMute(!mute);
}


void OptionMenu::mainMenuCallback(CCObject* pSender)
{
	CCScene *pScene = MainMenu::scene(true);
	CCDirector::sharedDirector()->replaceScene(CCTransitionMoveInL::create(0.5,pScene));
	//CCTransitionFade* transitionScene = CCTransitionFade::create(0.5, pScene,ccc3(0,0,100));
	//CCDirector::sharedDirector()->replaceScene(transitionScene);
}

