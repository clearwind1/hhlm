#ifndef __DIALOG_LOGONGIFT_H__
#define __DIALOG_LOGONGIFT_H__

#include "cocos2d.h"
#include "DialogBase.h"

USING_NS_CC;

#define LOGONGIFT_DAY_MAX		(7)

class DialogLogonGift : public DialogBase
{
public:
    virtual bool init(void);
	static DialogLogonGift *create(void);

 	virtual void closeClicked(void);

private:
	void close(void);
	void continueMenuCallback(CCObject *pSender);

private:
	CCSprite *logonSprite[LOGONGIFT_DAY_MAX];
};

#endif // __DIALOG_LOGONGIFT_H__
