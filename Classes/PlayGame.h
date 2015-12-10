#ifndef __PLAY_GAME_H__
#define __PLAY_GAME_H__


#include "cocos2d.h"
#include "LinkCard.h"

USING_NS_CC;

class PlayGame : public CCLayer
{
public:
	static CCScene *scene(int level, bool resume = false);

    virtual bool init(int level, bool resume = false);
	static PlayGame *create(int level, bool resume = false);

	static PlayGame *shared(void);

	void setLevel(int level, int option);

	//void nextLevel(void);
	//void restartLevel(void);
	//void restartLevelWithDoubleTime(void);

	virtual void keyBackClicked(void);
	virtual void keyMenuClicked(void);

    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void registerWithTouchDispatcher();

private:
	void setLevelCallBack(float dt);
	//void nextLevelCallBack(float dt);
	//void restartLevelCallBack(float dt);
	//void restartLevelWithDoubleTimeCallBack(float dt);

private:
	int m_curLevel;
	int m_curOption;
};


#endif // __PLAY_GAME_H__
