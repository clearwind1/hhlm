#ifndef __GAME_MENU_H__
#define __GAME_MENU_H__


#include "cocos2d.h"
#include "DigitSprite.h"
#include "UserData.h"

USING_NS_CC;

#if 1
enum
{
	GameMenuItemPause = 0,
	GameMenuItemRefresh,
	GameMenuItemHelp,
	GameMenuItemTime,
	GameMenuItemBomb,
	GameMenuItemMax
};

class GameMenu : public CCLayer
{

public:
    
    GameMenu();
    ~GameMenu();
    
    static GameMenu *shared();
    
    
	virtual bool init();
	static GameMenu *create();
    
    void setToolNum(int ktool,int num);

private:
	bool initWithMenu();
	void menuClickAction(CCObject* pSender);
	void pauseMenuCallback(CCObject* pSender);
	void refreshMenuCallback(CCObject* pSender);
	void helpMenuCallback(CCObject* pSender);
	void timeMenuCallback(CCObject* pSender);
	void bombMenuCallback(CCObject* pSender);

private:
	float m_scale;
	float m_xStart;
	float m_yStart;
	float m_width;
	float m_height;
    DigitSprite *m_toolsnumSprite[TOOS_NUM];
	CCSprite *m_selSprite;
	CCMenuItemImage *m_menuItem[GameMenuItemMax];
};

#endif
#endif // __GAME_MENU_H__
