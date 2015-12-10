
#include "CardSprite.h"

USING_NS_CC;


#define CARDSPRITE_WIDTH		(100)
#define CARDSPRITE_HEIGHT		(125)


static const int cardStyleBase[CardSpriteStyleMax] =
{
	0, 100, 200, 300, 400, 500, 1000
};

static const int cardStyleMax[CardSpriteStyleMax] =
{
	0, 15, 29, 32, 38, 40, 1
};

bool CardSprite::init(int style,int id)
{
	char buffer[128];

	if(style <= 0 || style >= CardSpriteStyleMax || id <= 0 || id > cardStyleMax[style])
		return false;

	snprintf(buffer,128,"images/cards/%d.png",cardStyleBase[style] + id);
	if(!CCSprite::initWithFile((const char *)buffer))
		return false;

	x = 0;
	y = 0;
	m_scale = 1.0f;
	m_id = id;
	m_style = style;
	m_flags = 0;
	m_hideCount = 0;
	m_freezeCount = 0;
	m_disableCount = 0;
	m_selectedSprite = NULL;
	m_hideSprite = NULL;
	m_aimSprite = NULL;
	m_blinkSprite = NULL;
	m_freezeSprite = NULL;

	return true;
}

CardSprite *CardSprite::create(int style,int id)
{
	CardSprite *linkSprite = new CardSprite();
	if(linkSprite != NULL && linkSprite->init(style,id))
	{
		linkSprite->autorelease();
		return linkSprite;
	}
	else
	{
		CC_SAFE_DELETE(linkSprite);
		return NULL;
	}
}

int CardSprite::getCardTotal(int style)
{
	if(style <= 0 || style > CardSpriteStyleMax)
		return 0;
	return cardStyleMax[style];
}

int CardSprite::getCardWidth()
{
	return CARDSPRITE_WIDTH - 18;
}

int CardSprite::getCardHeight()
{
	return CARDSPRITE_HEIGHT - 16;
}

void CardSprite::setScale(float scale)
{
	m_scale = scale;
	CCSprite::setScale(scale);
}

int CardSprite::getStyle()
{
	return m_style;
}

int CardSprite::getID()
{
	return m_id;
}

#if 0
void CardSprite::setID(int id)
{
	char buffer[128];

	if(id != 0)
	{
		m_id = id;
		snprintf(buffer,128,"images/fish_%02d_%02d.png",m_style,id);
		setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(buffer));
	}
}
#endif

void CardSprite::setSelected(bool selected, float duration)
{
	if(m_hideSprite != NULL)
		return;

	if(selected)
	{
		m_flags |= CardSpriteFlagSelected;
		if(m_selectedSprite == NULL)
		{
			m_selectedSprite = CCSprite::create("images/cards/effect_selected.png");
			m_selectedSprite->setAnchorPoint(ccp(0.0f, 0.0f));
			this->addChild(m_selectedSprite);
		}

		CCPoint pos = this->getPosition();
		CCActionInterval *moveTo1 = CCMoveTo::create(duration * 0.3f, ccp(pos.x + 5.0f, pos.y - 5.0f));
		CCActionInterval *moveTo2 = CCMoveTo::create(duration * 0.7f, pos);
		CCFiniteTimeAction *seq = CCSequence::create(moveTo1, moveTo2, NULL);
		this->runAction(seq);
	}
	else
	{
		m_flags &= ~CardSpriteFlagSelected;
		if(m_selectedSprite != NULL)
		{
			this->removeChild(m_selectedSprite,true);
			m_selectedSprite = NULL;
		}
	}
}

void CardSprite::setFreeze(bool freeze)
{
	if(freeze)
	{
		if(m_freezeCount++ != 0)
			return;
		m_flags |= CardSpriteFlagFreeze;
		m_disableCount++;
		m_freezeSprite = CCSprite::create("images/cards/effect_freeze.png");
		m_freezeSprite->setAnchorPoint(ccp(0.0f, 0.0f));
		this->addChild(m_freezeSprite);
		if(m_flags & CardSpriteFlagHide)
			m_freezeSprite->setVisible(false);
	}
	else
	{
		CCAssert(m_freezeCount > 0,"m_freezeCount error!");
		if(--m_freezeCount != 0)
			return;
		m_flags &= ~CardSpriteFlagFreeze;
		CCAssert(m_disableCount > 0,"setFadeInCallBack");
		m_disableCount--;
		this->removeChild(m_freezeSprite,true);
		m_freezeSprite = NULL;
	}
}

void CardSprite::hideCallBack(void)
{
	CCAssert(m_disableCount > 0,"setFadeInCallBack");
	m_disableCount--;
	this->removeChild(m_hideSprite,true);
	m_hideSprite = NULL;
	m_flags &= ~CardSpriteFlagHide;
	if(m_flags & CardSpriteFlagFreeze)
		m_freezeSprite->setVisible(true);
}

void CardSprite::setHide(bool hide, float duration)
{
	if(hide)
	{
		if(m_hideCount++ != 0)
			return;
		
		m_flags |= CardSpriteFlagHide;
		if(m_flags & CardSpriteFlagFreeze)
			m_freezeSprite->setVisible(false);

		m_disableCount++;
		//m_hideSprite = CCSprite::create("images/cards/effect_hide.png");
		m_hideSprite = CCSprite::create("images/cards/1001.png");
		m_hideSprite->setAnchorPoint(ccp(0.0f, 0.0f));
		this->addChild(m_hideSprite);

		if(duration != 0)
		{
			this->setOpacity(0);
			m_hideSprite->setOpacity(0);
			CCActionInterval *fadeIn = CCFadeIn::create(duration);
			m_hideSprite->runAction(fadeIn);
		}
	}
	else
	{
		CCAssert(m_hideCount > 0,"m_hideCount error!");
		if(--m_hideCount != 0)
			return;
		if(duration == 0)
		{
			CCAssert(m_disableCount > 0,"setFadeInCallBack");
			m_disableCount--;
			this->removeChild(m_hideSprite,true);
			m_hideSprite = NULL;
			m_flags &= ~CardSpriteFlagHide;
			this->setOpacity(255);
			if(m_flags & CardSpriteFlagFreeze)
				m_freezeSprite->setVisible(true);
		}
		else
		{
			//CCActionInterval *delayTime = CCDelayTime::create(1);
			this->setOpacity(255);
			CCScaleTo *scaleTo1 = CCScaleTo::create(duration * 0.7f,0,m_scale);
			CCScaleTo *scaleTo2 = CCScaleTo::create(duration * 0.3f,m_scale,m_scale);
			CCCallFunc *funCall = CCCallFunc::create(this, callfunc_selector(CardSprite::hideCallBack));
			//CCFiniteTimeAction *seq = CCSequence::create(delayTime, scaleTo1, funCall, scaleTo2, NULL);
			CCFiniteTimeAction *seq = CCSequence::create(scaleTo1, funCall, scaleTo2, NULL);
			this->runAction(seq);
		}
	}
}

void CardSprite::setAim(bool aim, float duration)
{
	if(aim)
	{
		m_flags |= CardSpriteFlagAim;
		if(duration == 0)
			duration = 0.2f;
		if(m_aimSprite == NULL)
		{
			m_aimSprite = CCSprite::create("images/cards/effect_aim.png");
			this->addChild(m_aimSprite);
			//m_aimSprite->setAnchorPoint(ccp(0.0f, 0.0f));
			//CCSize size = m_aimSprite->getContentSize() * m_scale;
			CCSize size = this->getContentSize();
			m_aimSprite->setPosition(ccp(size.width / 2, size.height / 2));
			m_aimSprite->setScale(1.1f);
			CCScaleTo *scaleTo = CCScaleTo::create(duration,1.0f);
			m_aimSprite->runAction(scaleTo);
		}
	}
	else
	{
		m_flags &= ~CardSpriteFlagAim;
		if(m_aimSprite != NULL)
		{
			this->removeChild(m_aimSprite,true);
			m_aimSprite = NULL;
		}
	}
}

void CardSprite::setBlink(bool blink, float duration)
{
	if(blink)
	{
		m_flags |= CardSpriteFlagBlink;
		if(duration == 0)
			duration = 0.3f;
		if(m_blinkSprite == NULL)
		{
			m_blinkSprite = CCSprite::create("images/cards/effect_blink.png");
			m_blinkSprite->setAnchorPoint(ccp(0.0f, 0.0f));
			this->addChild(m_blinkSprite);

			CCActionInterval *blink = CCBlink ::create(duration, 1);
			CCRepeatForever *repeatForever = CCRepeatForever::create((CCActionInterval *)blink);
			m_blinkSprite->runAction(repeatForever);
		}
	}
	else
	{
		m_flags &= ~CardSpriteFlagBlink;
		if(m_blinkSprite != NULL)
		{
			this->removeChild(m_blinkSprite,true);
			m_blinkSprite = NULL;
		}
	}
}

void CardSprite::setFadeInCallBack(float dt)
{
	CCAssert(m_disableCount > 0,"setFadeInCallBack");
	m_disableCount--;
	if(this->getOpacity() != 255)
		this->setOpacity(255);
}

void CardSprite::setFadeIn()
{
	m_disableCount++;
	this->setOpacity(0);
	CCActionInterval *fadeIn = CCFadeIn::create(1.0f);
	//CCCallFunc *funCall = CCCallFunc::create(this, callfunc_selector(CardSprite::setFadeInCallBack));
	//CCFiniteTimeAction *seq = CCSequence::create(fadeIn, funCall, NULL);
	this->scheduleOnce(schedule_selector(CardSprite::setFadeInCallBack), 1.05f);
	this->runAction(fadeIn);
}

bool CardSprite::getFlags(CardSpriteFlags flagMask)
{
	return ((m_flags & flagMask) == 0 ? false : true);
}

bool CardSprite::getDisable(void)
{
	if(m_style < CardSpriteStyleStart || m_style > CardSpriteStyleEnd)
		return true;
	return m_disableCount == 0 ? false : true;
}
