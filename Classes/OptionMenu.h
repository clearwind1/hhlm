#ifndef __OPTION_MENU_H__
#define __OPTION_MENU_H__

#include "cocos2d.h"

USING_NS_CC;

class OptionMenu : public CCLayer
{
public:
	static CCScene *scene();

    virtual bool init(); 
	static OptionMenu *create();

private:
	void initMenu();

	// a selector callback
	void musicMenuCallback(CCObject* pSender);
	void effectMenuCallback(CCObject* pSender);
	void mainMenuCallback(CCObject* pSender);

private:
	float m_menuScale;

};

#endif // __OPTION_MENU_H__
