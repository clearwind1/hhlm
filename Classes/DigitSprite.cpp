

#include "DigitSprite.h"


USING_NS_CC;

DigitSprite *DigitSprite::create(int mode)
{
	DigitSprite *pRet = new DigitSprite();
	if (pRet && pRet->init())
	{
		//pRet->m_spriteList = CCArray::create();
		//pRet->m_spriteList->retain();
		pRet->m_mode = mode;
		pRet->m_curValue = 0;
		pRet->m_step = 0;
		pRet->m_value = 0;
		pRet->m_width = 50;
		pRet->m_height = 80;
		pRet->m_opacity = 255;
		pRet->m_spriteBatchNode = CCSpriteBatchNode::create("images/digits.png");
		pRet->m_spriteBatchNode->setPosition(ccp(0,0));
		pRet->m_scale = pRet->CCSprite::getScale();
		pRet->addChild(pRet->m_spriteBatchNode);
		pRet->autorelease();
		return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

void DigitSprite::output(int value)
{
	m_curValue = value;
	m_value = value;
	output(value,0);
}

void DigitSprite::output(int value,int style)
{
	if(style == 0)
	{
		outputWithFormat("%d",value);
	}
	else
	{
		this->unschedule(schedule_selector(DigitSprite::updateOutput));
		m_curValue = m_value;
		m_value = value;
		m_step = (m_value - m_curValue) / 50;
		if(m_step == 0)
			m_step = 1;
		this->schedule(schedule_selector(DigitSprite::updateOutput),0.005f);
	}
}

void DigitSprite::updateOutput(float dt)
{
	if(m_curValue + m_step < m_value)
		m_curValue += m_step;
	else
		m_curValue = m_value;
	outputWithFormat("%d",m_curValue);
	if(m_curValue == m_value)
		this->unschedule(schedule_selector(DigitSprite::updateOutput));
}


void DigitSprite::output(const char *str)
{
	float start,width = 0;
	CCSprite *sprite;
	CCSize size;

	if(str == NULL)
		return;

	m_spriteBatchNode->removeAllChildren();
	//m_spriteList->removeAllObjects();

	size.width = 0;
	size.height = 0;
	while(*str)
	{
		size.height = m_height;
		if(*str >= '0' && *str <= '9')
		{
			start = ((*str - '0') * m_width);
			width = m_width;
		}
		else if(*str == '.')
		{
			start = (10 * m_width) + (0 * m_width);
			width = m_width;
		}
		else if(*str == ':')
		{
			start = (10 * m_width) + (1 * m_width);
			width = m_width;
		}
		else if(*str == '+')
		{
			start = (10 * m_width) + (2 * m_width);
			width = m_width;
		}
		else if(*str == '-')
		{
			start = (10 * m_width) + (3 * m_width);
			width = m_width;
		}
		else if(*str == '*')
		{
			start = (10 * m_width) + (4 * m_width);
			width = m_width;
		}
		else if(*str == '/')
		{
			start = (10 * m_width) + (5 * m_width);
			width = m_width;
		}
		else
		{
			str++;
			continue;
		}

		sprite = CCSprite::createWithTexture(m_spriteBatchNode->getTexture(),CCRect(start,0,width,m_height));
		sprite->setPosition(ccp(size.width + width / 2,0 + m_height / 2));
		sprite->setOpacity(m_opacity);
		m_spriteBatchNode->addChild(sprite);
		//m_spriteList->addObject(sprite);
		size.width += width;
		str++;
	}
	this->setContentSize(size);

#if 0
	this->stopAllActions();
	this->setScale(m_scale);
	CCActionInterval *scaleTo = CCScaleTo::create(0.1f,m_scale * 2);
	CCActionInterval *scaleTo1 = CCScaleTo::create(0.1f,m_scale);
	CCCallFunc *funCall= CCCallFunc::create(this, callfunc_selector(DigitSprite::ActionEnd));
	CCFiniteTimeAction *seq = CCSequence::create(scaleTo,scaleTo1,funCall,NULL);
	this->runAction(seq);
#endif
}

void DigitSprite::ActionEnd(void)
{
	this->setScale(m_scale);
}

void DigitSprite::outputWithFormat(const char *fmt,...)
{
    va_list args;
	char temp[128];

    va_start(args,fmt);
	vsnprintf(temp,128,fmt,args);
    va_end(args);
	output(temp);

}

GLubyte DigitSprite::getOpacity(void)
{
	return m_opacity;
}

void DigitSprite::setOpacity(GLubyte opacity)
{
	m_opacity = opacity;
#if 0
	for(unsigned int i = 0; i < m_spriteList->count(); i++) 
	{
		CCSprite *sprite = (CCSprite *)m_spriteList->objectAtIndex(i);
		sprite->setOpacity(m_opacity);
	}
#endif
}

void DigitSprite::setScale(float scale)
{
	m_scale = scale;
	CCSprite::setScale(scale);
}

float DigitSprite::getScale(void)
{
	return m_scale;
}

void DigitSprite::setSizeX(float x)
{
	this->setScale(x / this->getContentSize().width);
}

void DigitSprite::setSizeY(float y)
{
	this->setScale(y / this->getContentSize().height);
}

float DigitSprite::getSizeX()
{
	return this->getContentSize().width * this->getScaleX();
}

float DigitSprite::getSizeY()
{
	return this->getContentSize().height * this->getScaleY();
}

void DigitSprite::onExit()
{
}

DigitSprite::DigitSprite()
	: m_mode(0)
	, m_value(0)
	, m_curValue(0)
	, m_step(0)
	, m_width(0)
	, m_height(0)
	, m_scale(0)
	, m_opacity(0)
	,m_spriteBatchNode(NULL)
{

}
