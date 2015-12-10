#ifndef __DIALOGPAUSE_H__
#define __DIALOGPAUSE_H__

#include "cocos2d.h"
#include "DialogBase.h"


class DialogPause : public DialogBase
{
public:
    virtual bool init();  

	static CCScene* scene();

    CREATE_FUNC(DialogPause);

	virtual void closeClicked(void);

private:
	void close(void);
	void mainMenuMenuCallback(CCObject *pSender);
	void continueMenuCallback(CCObject *pSender);
	void restartMenuCallback(CCObject *pSender);

private:
	bool m_musicMute;
	bool m_effectMute;
};

#endif // __DIALOGPAUSE_H__
