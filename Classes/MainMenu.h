#ifndef __MAIN_MENU_H__
#define __MAIN_MENU_H__

#include "cocos2d.h"

USING_NS_CC;

class MainMenu : public CCLayer
{
public:
 	static CCScene* scene(bool back = false);

	virtual bool init(bool back); 
	static MainMenu *create(bool back);

	virtual void keyBackClicked(void);

private:
	void initMenu();

	// a selector callback
	void newMenuCallback(CCObject* pSender);

private:
	float m_menuScale;
};

#endif // __MAIN_MENU_H__
