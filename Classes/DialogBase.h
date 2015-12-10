#ifndef __DIALOG_BASE_H__
#define __DIALOG_BASE_H__

#include "cocos2d.h"

USING_NS_CC;

class DialogBase : public CCLayer
{
public:
	/*
	 width:�������Ļ��ȵı���,��Χ(0.0f < width <= 1.0f)
	 height:�������Ļ�߶ȵı���,��Χ(0.0f < height <= 1.0f)
	 titleImage:�Ի���ı���ͼƬ�ļ���
	*/
	virtual bool init();
	virtual bool init(float width, float height, float titleHeight);
	virtual bool init(const char *titleImage, float width, float height, float titleHeight);
	virtual bool init(const char *titleImage, const char *titleBgImage,
		const char *closeImage, const char *closePressedImage,
		const char *clientBgImage, float width, float height, float titleHeight);
	static DialogBase *create();
	static DialogBase *create(float width, float height, float titleHeight);
	static DialogBase *create(const char *titleImage, float width, float height, float titleHeight);
	static DialogBase *create(const char *titleImage, const char *titleBgImage,
		const char *closeImage, const char *closePressedImage,
		const char *clientBgImage, float width, float height, float titleHeight);

	CCSprite *getTitle(void);
	void setTitle(CCSprite *titleSprite);

	virtual const CCSize &getSize(void);
	virtual const CCSize &getTitleSize(void);
	virtual const CCSize &getClientSize(void);
	virtual const CCPoint &getClientBasePoint(void);

	virtual void closeClicked(void);
	virtual void show(void);
	virtual void hide(void);

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();
	virtual void keyBackClicked(void);

private:
	void menuCloseCallback(CCObject *pSender);

private:
	CCSize m_titleSize;
	CCSize m_dialogSize;
	CCSize m_clientSize;
	CCPoint m_clientBasePoint;//����(OPENGL)
	CCSprite *m_titleSprite;
};

#endif // __DIALOG_BASE_H__
