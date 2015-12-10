#include "Lightning.h"

USING_NS_CC;

#if 0
#define LIGHTNING_LINE_WIDTH_MAX		(512)
#define LIGHTNING_LINE_HEIGHT_MAX	(605)
#define LIGHTNING_LINE_ITEM_WIDTH	(128)
#define LIGHTNING_LINE_ITEM_HEIGHT	LIGHTNING_LINE_HEIGHT_MAX
#define LIGHTNING_LINE_ITEM_MAX		(4)
#define LIGHTNING_LINE_SIZE			(0.12f)
#else
#define LIGHTNING_LINE_WIDTH_MAX			(72)
#define LIGHTNING_LINE_HEIGHT_MAX			(360)
#define LIGHTNING_LINE_ITEM_WIDTH			(24)
#define LIGHTNING_LINE_ITEM_HEIGHT			LIGHTNING_LINE_HEIGHT_MAX
#define LIGHTNING_LINE_ITEM_MAX				(3)
#define LIGHTNING_LINE_SIZE					(0.04f)
#endif

#define LIGHTNING_ENDPOINT_WIDTH_MAX		(500)
#define LIGHTNING_ENDPOINT_HEIGHT_MAX		(100)
#define LIGHTNING_ENDPOINT_ITEM_SIZE		(100)
#define LIGHTNING_ENDPOINT_ITEM_MAX			(5)


bool Lightning::init(CCLayer *gameLayer, int bottom, int top)
{
    if(!CCNode::init())
        return false;

	m_lightLineCount = 0;
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	m_xScale = size.width / LIGHTNING_LINE_ITEM_WIDTH * LIGHTNING_LINE_SIZE;

#if 0
	for(int i = 0; i < LightnangLineFrameMax; i++)
	{
		CCString *str = CCString::createWithFormat("images/lightning_line_%d.png", i + 1);
		m_lineSpriteFrame[i] = CCSprite::create(str->getCString());
		this->addChild(m_lineSpriteFrame[i]);
	}

	for(int i = 0; i < LightnangEndpointFrameMax; i++)
	{
		CCString *str = CCString::createWithFormat("lighting_endpoint_%d.png", i + 1);
		m_endpointSpriteFrame[i] = CCSprite::create(str->getCString());
		this->addChild(m_endpointSpriteFrame[i]);
	}
#endif

	m_lineSpriteBatchNode = CCSpriteBatchNode::create("images/lightning_line.png");
	m_lineSpriteBatchNode->setPosition(ccp(0,0));
	this->addChild(m_lineSpriteBatchNode);

	m_endpointSpriteBatchNode = CCSpriteBatchNode::create("images/lightning_endpoint.png");
	m_endpointSpriteBatchNode->setPosition(ccp(0,0));
	this->addChild(m_endpointSpriteBatchNode);

    return true;
}

Lightning *Lightning::create(CCLayer *gameLayer, int bottom, int top)
{
	Lightning *lightning = new Lightning();
	if(lightning != NULL && lightning->init(gameLayer, bottom, top))
	{
		lightning->autorelease();
		return lightning;
	}
	else
	{
		CC_SAFE_DELETE(lightning);
		return NULL;
	}
}

void Lightning::setLighting(int index,const CCPoint &pos1,const CCPoint &pos2)
{
	if(index < LightnangLightLineMax)
	{
		m_lightLine[index].p1 = pos1;
		m_lightLine[index].p2 = pos2;
		m_lightLineCount = index + 1;
	}
}

void Lightning::drawLighting(float interval)
{
	if(m_lightLineCount != 0)
	{
		if(interval == 0)
			interval = 9.9f;
		displayAnimateEffect(interval);
		m_lightLineCount = 0;
	}
}


void Lightning::stopLighting(void)
{
	displayLineAnimateEffectCall();
	displayEndpointAnimateEffectCall();
}


void Lightning::displayAnimateEffect(float interval)
{
	m_lineSpriteBatchNode->removeAllChildren();
	m_endpointSpriteBatchNode->removeAllChildren();
	for(int i = 0;i < m_lightLineCount;i++)
	{
		displayLineAnimateEffect(i, interval);
	}
	if(m_lightLineCount == 1)
	{
		displayEndpointAnimateEffect(m_lightLine[0].p1, interval);
		displayEndpointAnimateEffect(m_lightLine[0].p2, interval);
	}
	else
	{
		CCPoint point1 = m_lightLine[0].p1;
		CCPoint point2 = m_lightLine[0].p2;
		if((point1.x != m_lightLine[1].p1.x || point1.y != m_lightLine[1].p1.y) 
			&& (point1.x != m_lightLine[1].p2.x && point1.y != m_lightLine[1].p2.y))
			displayEndpointAnimateEffect(point1, interval);
		else
			displayEndpointAnimateEffect(point2, interval);

		point1 = m_lightLine[m_lightLineCount - 1].p1;
		point2 = m_lightLine[m_lightLineCount - 1].p2;
		if((point1.x != m_lightLine[m_lightLineCount - 2].p1.x || point1.y != m_lightLine[m_lightLineCount - 2].p1.y) 
			&& (point1.x != m_lightLine[m_lightLineCount - 2].p2.x && point1.y != m_lightLine[m_lightLineCount - 2].p2.y))
			displayEndpointAnimateEffect(point1, interval);
		else
			displayEndpointAnimateEffect(point2, interval);
	}
}

void Lightning::displayLineAnimateEffect(int index,float interval)
{
	float xLen = m_lightLine[index].p2.x -  m_lightLine[index].p1.x;
	float yLen = m_lightLine[index].p2.y -  m_lightLine[index].p1.y;

	//if(xLen == 0 && yLen == 0)
	//	return;
	//if(xLen != 0 && yLen != 0)
	//	return;

	CCLog("displayLineAnimateEffect,sizeof(float):%d,sizeof(double):%d", sizeof(float), sizeof(double));
	CCLog("displayLineAnimateEffect,xLen(%f) = p2.x(%f) - p1.x(%f)", xLen, m_lightLine[index].p2.x,  m_lightLine[index].p1.x);
	CCLog("displayLineAnimateEffect,yLen(%f) = p2.y(%f) - p1.y(%f)", yLen, m_lightLine[index].p2.y,  m_lightLine[index].p1.y);

	CCSprite *sprite = CCSprite::createWithTexture(m_lineSpriteBatchNode->getTexture(),CCRect(0,0,LIGHTNING_LINE_ITEM_WIDTH,LIGHTNING_LINE_ITEM_HEIGHT));
	sprite->setScaleX(m_xScale);
	if(xLen != 0)
	{
		CCLog("xLen != 0,fabs(xLen):%f, scaleY:%f", fabs(xLen), fabs(xLen) / LIGHTNING_LINE_ITEM_HEIGHT);
		sprite->setScaleY(fabs(xLen) / LIGHTNING_LINE_ITEM_HEIGHT);
		sprite->setPosition(ccp(m_lightLine[index].p1.x + xLen / 2,m_lightLine[index].p1.y));
		sprite->setRotation(90.0f);
	}
	else
	{
		CCLog("xLen == 0,fabs(yLen):%f, scaleY:%f", fabs(yLen), fabs(yLen) / LIGHTNING_LINE_ITEM_HEIGHT);
		sprite->setScaleY(fabs(yLen) / LIGHTNING_LINE_ITEM_HEIGHT);
		sprite->setPosition(ccp(m_lightLine[index].p1.x,m_lightLine[index].p1.y + yLen / 2));
	}
	m_lineSpriteBatchNode->addChild(sprite);

	CCAnimation *animation = CCAnimation::create();
	animation->setDelayPerUnit(0.15f / LIGHTNING_LINE_ITEM_MAX);
	animation->setRestoreOriginalFrame(true);
	animation->setLoops(interval / 0.2f);
	for(int item = 0; item < LIGHTNING_LINE_ITEM_MAX; item++)
	{
		animation->addSpriteFrameWithTexture(m_lineSpriteBatchNode->getTexture(),CCRect(item * LIGHTNING_LINE_ITEM_WIDTH,0,LIGHTNING_LINE_ITEM_WIDTH,LIGHTNING_LINE_ITEM_HEIGHT));
	}

	CCAnimate *animate = CCAnimate::create(animation);  
	//sprite->runAction(animate);
	CCCallFunc *funCall= CCCallFunc::create(this, callfunc_selector(Lightning::displayLineAnimateEffectCall));
	CCFiniteTimeAction *seq = CCSequence::create(animate,funCall,NULL);
	sprite->runAction(seq);
	//m_lineSpriteBatchNode->runAction(seq);
}

void Lightning::displayLineAnimateEffectCall()
{
	m_lineSpriteBatchNode->removeAllChildren();
}


void Lightning::displayEndpointAnimateEffect(const CCPoint &point, float interval)
{
	CCSprite *sprite = CCSprite::createWithTexture(m_endpointSpriteBatchNode->getTexture(),CCRect(0,0,LIGHTNING_ENDPOINT_ITEM_SIZE,LIGHTNING_ENDPOINT_ITEM_SIZE));
	sprite->setScale(m_xScale);
	sprite->setPosition(point);
	m_endpointSpriteBatchNode->addChild(sprite);

	CCAnimation *animation = CCAnimation::create();
	animation->setDelayPerUnit(0.15f / LIGHTNING_LINE_ITEM_MAX);
	animation->setRestoreOriginalFrame(true);
	animation->setLoops(interval / 0.2f);
	for(int item = 0; item < LIGHTNING_LINE_ITEM_MAX; item++)
	{
		animation->addSpriteFrameWithTexture(m_endpointSpriteBatchNode->getTexture(),CCRect(item * LIGHTNING_ENDPOINT_ITEM_SIZE,0,LIGHTNING_ENDPOINT_ITEM_SIZE,LIGHTNING_ENDPOINT_ITEM_SIZE));
	}

	CCAnimate *animate = CCAnimate::create(animation);  
	CCCallFunc *funCall= CCCallFunc::create(this, callfunc_selector(Lightning::displayEndpointAnimateEffectCall));
	CCFiniteTimeAction *seq = CCSequence::create(animate,funCall,NULL);
	sprite->runAction(seq);
}


void Lightning::displayEndpointAnimateEffectCall()
{
	m_endpointSpriteBatchNode->removeAllChildren();
}
