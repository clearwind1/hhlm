#ifndef __CARD_SPRITE_H__
#define __CARD_SPRITE_H__

#include "cocos2d.h"

USING_NS_CC;

enum
{
	CardSpriteStyleNone = 0,
	CardSpriteStyleSweet,
	CardSpriteStyleAnimal,
	CardSpriteStyleFish,
	CardSpriteStyleFruit,
	CardSpriteStyleCrop,
	CardSpriteStyleSpecial,
	CardSpriteStyleMax
};

#define CardSpriteStyleStart		CardSpriteStyleAnimal
#define CardSpriteStyleEnd			CardSpriteStyleCrop

enum
{
	CardSpriteSpecialIdDisable = 1,
	CardSpriteSpecialIdMax
};

typedef enum
{
	CardSpriteFlagSelected = 1,
	CardSpriteFlagHide = 2,
	CardSpriteFlagFreeze = 4,
	CardSpriteFlagAim = 8,
	CardSpriteFlagBlink = 16,
}CardSpriteFlags;

class CardSprite : public CCSprite
{
public:
	int x;
	int y;

public:
	bool init(int style,int id);
	static CardSprite *create(int style,int id);

	static int getCardTotal(int style);
	static int getCardWidth();
	static int getCardHeight();

	virtual void setScale(float scale);

	int getID();
	int getStyle();

	void setSelected(bool selected, float duration = 0.07f);
	void setHide(bool hide, float duration = 0.5f);
	void setAim(bool aim, float duration = 0.2f);
	void setBlink(bool blink, float duration = 0.3f);
	void setFreeze(bool freeze);
	void setFadeIn();

	bool getFlags(CardSpriteFlags flagMask);
	bool getDisable(void);

private:
	void setFadeInCallBack(float dt);
	void hideCallBack(void);

private:
	int m_style;
	int m_id;
	int m_flags;
	int m_hideCount;
	int m_freezeCount;
	int m_disableCount;
	float m_scale;
	CCSprite *m_selectedSprite;
	CCSprite *m_freezeSprite;
	CCSprite *m_hideSprite;
	CCSprite *m_aimSprite;
	CCSprite *m_blinkSprite;
};

#endif //__CARD_SPRITE_H__