
#include "ActionInterval.h"

USING_NS_CC;

FadeIn* FadeIn::create(float d, GLubyte opacity)
{
    FadeIn* pAction = new FadeIn();

	pAction->m_opacity = opacity;
    pAction->initWithDuration(d);
    pAction->autorelease();

    return pAction;
}

CCObject* FadeIn::copyWithZone(CCZone *pZone)
{
    CCZone* pNewZone = NULL;
    FadeIn* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject)
    {
        //in case of being called at sub class
        pCopy = (FadeIn*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new FadeIn();
        pZone = pNewZone = new CCZone(pCopy);
    }
    
    CCActionInterval::copyWithZone(pZone);

    CC_SAFE_DELETE(pNewZone);

    return pCopy;
}

void FadeIn::update(float time)
{
    CCRGBAProtocol *pRGBAProtocol = dynamic_cast<CCRGBAProtocol*>(m_pTarget);
    if (pRGBAProtocol)
    {
        pRGBAProtocol->setOpacity((GLubyte)(m_opacity * time));
    }
    /*m_pTarget->setOpacity((GLubyte)(255 * time));*/
}

CCActionInterval* FadeIn::reverse(void)
{
    return FadeOut::create(m_fDuration);
}

//
// FadeOut
//

FadeOut* FadeOut::create(float d, GLubyte opacity)
{
    FadeOut* pAction = new FadeOut();

	pAction->m_opacity = opacity;
    pAction->initWithDuration(d);
    pAction->autorelease();

    return pAction;
}

CCObject* FadeOut::copyWithZone(CCZone *pZone)
{
    CCZone* pNewZone = NULL;
    FadeOut* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (FadeOut*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new FadeOut();
        pZone = pNewZone = new CCZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    CC_SAFE_DELETE(pNewZone);

    return pCopy;
}

void FadeOut::update(float time)
{
    CCRGBAProtocol *pRGBAProtocol = dynamic_cast<CCRGBAProtocol*>(m_pTarget);
    if (pRGBAProtocol)
    {
        pRGBAProtocol->setOpacity(GLubyte(m_opacity * (1 - time)));
    }
    /*m_pTarget->setOpacity(GLubyte(255 * (1 - time)));*/    
}

CCActionInterval* FadeOut::reverse(void)
{
    return FadeIn::create(m_fDuration);
}
