#ifndef __DIALOG_GAME_END_H__
#define __DIALOG_GAME_END_H__

#include "cocos2d.h"
#include "DialogBase.h"

USING_NS_CC;

enum
{
	DialogGameEndModePass = 0,
	DialogGameEndModeFail,
	DialogGameEndModeMax
};

class DialogGameEnd : public DialogBase
{
public:
    virtual bool init(int mode, int level, int stars, int time);
	static DialogGameEnd *create(int mode, int level, int stars, int time);

 	virtual void closeClicked(void);

private:
	void close(void);
	void backMenuCallback(CCObject *pSender);
	void nextMenuCallback(CCObject *pSender);
	void restartMenuCallback(CCObject *pSender);

private:
	int m_mode;
};

#endif // __DIALOG_GAME_END_H__
