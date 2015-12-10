#ifndef __DIALOG_SETTING_H__
#define __DIALOG_SETTING_H__

#include "cocos2d.h"
#include "DialogBase.h"

USING_NS_CC;

class DialogSetting : public DialogBase
{
public:
    virtual bool init(void);
	static DialogSetting *create(void);

 	virtual void closeClicked(void);

private:
	void close(void);
	void backMenuCallback(CCObject *pSender);
	void restartMenuCallback(CCObject *pSender);
	void musicMenuCallback(CCObject *pSender);
	void effectMenuCallback(CCObject *pSender);

};

#endif // __DIALOG_SETTING_H__
