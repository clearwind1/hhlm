#ifndef __GAME_INFO_H__
#define __GAME_INFO_H__


#include "cocos2d.h"
#include "DigitSprite.h"
#include "AppConfig.h"

USING_NS_CC;

enum
{
	GameInfoTimeLevelWarning = 0,
	GameInfoTimeLevelPrompt,
	GameInfoTimeLevelNormal,
	GameInfoTimeLevelMax
};


#define GAMEINFO_SCORE_LEVEL_SIZE		(5)
#define GAMEINFO_SCORE_LEVEL_MAX		(4)

class GameInfo : public CCLayer
{

public:
	static GameInfo *shared();
	GameInfo();
	~GameInfo();

	virtual bool init();
	static GameInfo *create();

	void setTime(int time);
	void setTime(int time,int timeDown);

	void setScoreLevel(int level);

	void setLevel(int level);
	void setCoin(int coin);

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();

private:
	bool initWithLevelAndTime();
	bool initWithCoin();
	bool initWithScore();

	void coinMenuCallback(CCObject* pSender);

private:
	float m_scale;
	float m_xStart;
	float m_yStart;
	float m_width;
	float m_height;
	int m_scoreLevel;
	int m_timeDown;
	CCSprite *m_timeProgSprite[GameInfoTimeLevelMax];
	CCProgressTimer *m_timeProgBar;
	DigitSprite *m_timeDownDigitSprite;
	DigitSprite *m_levelDigitSprite;
	DigitSprite *m_coinDigitSprite;
    
	CCSprite *m_scoreScore;
	CCSprite *m_scoreProgSprite[GAMEINFO_SCORE_LEVEL_MAX];
	CCProgressTimer *m_scoreProgBar;
};

#endif // __GAME_INFO_H__
