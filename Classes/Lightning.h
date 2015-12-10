#ifndef __LIGHTNING_H__
#define __LIGHTNING_H__

#include "cocos2d.h"

#define LightnangLightLineMax		(3)

#define LightnangLineFrameMax		(3)
#define LightnangEndpointFrameMax	(5)


USING_NS_CC;

struct LightLine
{
	CCPoint p1;
	CCPoint p2;
};

class Lightning : public CCNode
{
public:
	virtual bool init(CCLayer *gameLayer, int bottom, int top);
	static Lightning *create(CCLayer *gameLayer, int bottom, int top);

	void setLighting(int index,const CCPoint &pos1,const CCPoint &pos2);
	void drawLighting(float interval = 0);
	void stopLighting(void);

private:
	void displayAnimateEffect(float interval);
	void displayLineAnimateEffect(int index,float interval);
	void displayLineAnimateEffectCall();
	void displayEndpointAnimateEffect(const CCPoint &point, float interval);
	void displayEndpointAnimateEffectCall();

private:
	float m_xScale;

	int m_lightLineCount;
	CCSprite *m_lineSpriteFrame[LightnangLineFrameMax];
	CCSprite *m_endpointSpriteFrame[LightnangEndpointFrameMax];
	CCSpriteBatchNode *m_lineSpriteBatchNode;
	CCSpriteBatchNode *m_endpointSpriteBatchNode;
	struct LightLine m_lightLine[LightnangLightLineMax];
};

#endif // __LIGHTNING_H__
