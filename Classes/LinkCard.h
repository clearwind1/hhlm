#ifndef __LINK_CARD_H__
#define __LINK_CARD_H__


#include "cocos2d.h"
#include "DigitSprite.h"
#include "CardSprite.h"
#include "AppConfig.h"
#include "Lightning.h"
#include "LinkMap.h"

#define LinkCardLinkLineMax			(3)

#define LinkCardPropPriceRefresh		(10)
#define LinkCardPropPriceBomb			(15)
#define LinkCardPropPriceHelp			(5)
#define LinkCardPropPriceTime			(20)
#define LinkCardPropPriceNext			(50)
#define LinkCardPropPriceDoubleTime		(30)

USING_NS_CC;

typedef enum
{
	LinkCardInitModeDefault = 0,
	LinkCardInitModeNew,
	LinkCardInitModeRestart,
	LinkCardInitModeDoubleTime,
	LinkCardInitModeResume,
	LinkCardInitModeNext,
	LinkCardInitModeMax,
}LinkLinerInitMode;

enum
{
	LinkCardPropSucceed = 0,
	LinkCardPropError,
	LinkCardPropNotEnough,
	LinkCardPropMax
};

struct LinkLine
{
	int x1;
	int y1;
	int x2;
	int y2;
};

class LinkCard : public CCLayer
{
public:
    
    ~LinkCard();

    virtual bool init(int level, int mode = LinkCardInitModeDefault);
	static LinkCard *create(int level, int mode = LinkCardInitModeDefault);

	static LinkCard *shared(void);

    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void registerWithTouchDispatcher();

	virtual void keyBackClicked(void);

	int refreshProp(void);
	int bombProp(void);
	int helpProp(void);
	int timeProp(void);
	int nextLevelProp(void);
	int doubleTimeProp(void);

	int gameStore(int kshop,bool gift = false);
	int backGameWithStore(void);
	int pauseGame(void);
	int resumeGame(void);
	int nextGame(void);
	int restartGame(void);
	int restartGameWithDoubleTime(void);

	void showFailDialog(void);

	int getMaxTime();
	void setMaxTime(int msec);

	int getTime();
	void setTime(int msec);


private:

    void initWithLayout(void);
    bool initWithResume(void);
    bool initWithLink(void);

	void gameGoStart(float dt);
	void gameStartCallBack(float dt);
	void timeUpdateCallBack(float dt);
	void clearLinkCardCallBack(float dt);

	void gameOver(void);
	void gamePass(void);

	void goLevelStart(void);
	void goLevelStartCallBack(void);

	void goLevelEnd(bool isPass);
	void goLevelEndPassCallBack(void);
	void goLevelEndFailCallBack(void);

	void clearFreezeCard(CardSprite *cardSprite);
	bool checkNormalCard(CardSprite *cardSprite);

	bool addNewCard(void);

	const CCPoint getPos(int x,int y);
	bool getXY(const CCPoint &pos, int *x, int *y);
	int getID(int x,int y);
	int getIDEx(int x,int y);
	void setLinkLine(int index,int x1,int y1,int x2,int y2);
	bool isAdj(int x1,int y1,int x2,int y2,bool updatePos = false);
	bool isLine(int x1,int y1,int x2,int y2,bool start,bool end,bool updatePos = false);
	bool isLineToLine(int x1,int y1,int x2,int y2,bool updatePos = false);
	bool isLineToLineToLine(int x1,int y1,int x2,int y2,bool updatePos = false);
	bool isLink(int x1,int y1,int x2,int y2,bool updatePos = false);
	bool searchLink(int *x1,int *y1,int *x2,int *y2);
	bool checkLink(void);
	void clearLink(int x1,int y1,int x2,int y2);

	void clearSelSprite(void);
	void refreshCardCallBack(float dt);
	bool refreshCard(bool action);
	void bombCardCallBack(float dt);
	bool bombCard(void);
	bool helpCard(void);
	bool addTime(void);

	void scoreLevelHandler(float dt);

private:
	int m_level;
	int m_style;
	int m_cardStyle;
	int m_timeDown;
	int m_timeDownMax;
	int m_timeCount;
	int m_scoreCount;
	int m_scoreLevel;
	int m_count;
	int m_addCount;
	bool m_lock;
	bool m_clearLock;
	float m_width;
	float m_height;
	float m_scale;
	float m_xStart;
	float m_yStart;
	int m_xCards;
	int m_yCards;
	int m_linkLineCount;
	struct LinkLine m_linkLine[LinkCardLinkLineMax];
	CardSprite *m_linkSel;
	CardSprite *m_linkSel2;
	CardSprite *m_linkHelp1;
	CardSprite *m_linkHelp2;
	CardSprite *m_clearLink;
	CardSprite *m_clearLink2;
	LinkMap *m_cardMap;
	Lightning *m_lightning;
	CardSprite *m_linkSprite[LINK_MAP_MAX];
};

#endif // __LINK_CARD_H__
