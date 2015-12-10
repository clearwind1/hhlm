
#ifndef __ACTION_INTERVAL_H__
#define __ACTION_INTERVAL_H__

#include "cocos2d.h"

USING_NS_CC;


class FadeIn : public CCActionInterval
{
public:
    virtual void update(float time);
    virtual CCActionInterval* reverse(void);

    virtual CCObject* copyWithZone(CCZone* pZone);

public:
    static FadeIn* create(float d, GLubyte opacity = 255);

private:
	GLubyte m_opacity;
};

class FadeOut : public CCActionInterval
{
public:
    virtual void update(float time);
    virtual CCActionInterval* reverse(void);

    virtual CCObject* copyWithZone(CCZone* pZone);

public:
    static FadeOut* create(float d, GLubyte opacity = 255);

private:
	GLubyte m_opacity;
};


#endif //__ACTION_INTERVAL_H__
