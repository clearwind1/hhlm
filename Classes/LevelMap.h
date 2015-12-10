#ifndef __LEVEL_MAP_H__
#define __LEVEL_MAP_H__

#include "cocos2d.h"
#include "DigitSprite.h"

USING_NS_CC;

#define LEVEL_MAP_MAX			(11)
#define LEVEL_MAP_LOCK_MAX		(99)	//LINK_LEVEL_MAX
#define LEVEL_MAP_MOVEPOS_MAX		(9)

class LevelMap : public cocos2d::CCLayer
{
public:
    
    LevelMap();
    ~LevelMap();
    
    static LevelMap *shared();
    
    
    virtual bool init(); 
    static cocos2d::CCScene* scene();
	static LevelMap *create();

    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void registerWithTouchDispatcher();

	virtual void keyBackClicked(void);
    
    void setnumSprite();

private:
	void superGiftCallBack(float dt);
	void logonGiftStar();
	void timeUpdateCallBack(float dt);
	void startMovePos(float dt);
	void setPos(float pos);
	void startLevel(int level);
	void backMenuCallback(CCObject* pSender);
	void settingMenuCallback(CCObject* pSender);
    void openShopMenuCallback(CCObject* pSender);

private:
	bool m_startFlag;
	int m_touchCount;
	int m_selLevel;
	int m_curLevel;
	float m_scale;
	float m_pos;
	float m_endPos;
	float m_step;
	float m_beganY;
	float m_moveY;
	CCSize m_imgSize;
	CCSize m_winSize;
	CCSize m_mapSize;
	//CCSprite *m_selLevel;
	CCSpriteBatchNode *m_lockInfoSpriteBatchNode;
	CCSpriteBatchNode *m_lockSpriteBatchNode;
	CCSpriteBatchNode *m_unlockSpriteBatchNode;
	CCSprite *m_keySprite;
	CCSprite *m_map[LEVEL_MAP_MAX];
	CCSprite *m_lock[LEVEL_MAP_LOCK_MAX];
	int m_MovePosIndex;
	float m_MovePos[LEVEL_MAP_MOVEPOS_MAX];
    
    DigitSprite* m_startnumSprite;
    DigitSprite* m_virgoutnumSprite;
    
};

#endif // __LEVEL_MAP_H__
