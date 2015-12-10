#ifndef __DIALOG_SUPERGIFT_H__
#define __DIALOG_SUPERGIFT_H__

#include "cocos2d.h"
#include "DialogBase.h"

USING_NS_CC;

class DialogSuperGift : public DialogBase
{
public:
    virtual bool init(void);
	static DialogSuperGift *create(void);

 	virtual void closeClicked(void);

private:
	void close(void);
	void backMenuCallback(CCObject *pSender);
	void continueMenuCallback(CCObject *pSender);
    
private:
    
    int m_buyid;

};

#endif // __DIALOG_SUPERGIFT_H__
