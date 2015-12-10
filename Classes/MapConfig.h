#ifndef __MAP_CONFIG_H__
#define __MAP_CONFIG_H__

#include "AppConfig.h"

#if (MAP_EDITOR != 0)
#include "cocos2d.h"
#include "DigitSprite.h"

USING_NS_CC;

#define MAP_CONFIG_MAX			(11)

class MapConfig : public cocos2d::CCLayer
{
public:
    virtual bool init(); 
    static cocos2d::CCScene* scene();
	static MapConfig *create();

	void setPos(float pos);

    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void registerWithTouchDispatcher();

	virtual void keyBackClicked(void);

private:
	bool writeData(void);
	void addMenuCallback(CCObject* pSender);
	void okMenuCallback(CCObject* pSender);
	void timeUpdateCallBack(float dt);
	void startMovePos(float dt);

private:
	int m_touchCount;
	float m_scale;
	float m_pos;
	float m_endPos;
	float m_step;
	float m_beganY;
	float m_moveY;
	CCSize m_imgSize;
	CCSize m_winSize;
	CCSize m_mapSize;
	DigitSprite *m_lockDigitSprite;
	CCArray *m_lockArray;
	CCSprite *m_curLock;
	CCSpriteBatchNode *m_spriteBatchNode;
	CCSprite *m_map[MAP_CONFIG_MAX];
};
#endif
#endif // __MAP_CONFIG_H__
