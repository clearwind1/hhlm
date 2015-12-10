#ifndef __DIALOG_TIMEOUT_H__
#define __DIALOG_TIMEOUT_H__

#include "cocos2d.h"
#include "DialogBase.h"

USING_NS_CC;

class DialogTimeout : public DialogBase
{
public:
    virtual bool init(void);
	static DialogTimeout *create(void);

 	virtual void closeClicked(void);

private:
	void close(void);
	void backMenuCallback(CCObject *pSender);
	void restartMenuCallback(CCObject *pSender);

};

#endif // __DIALOG_TIMEOUT_H__
