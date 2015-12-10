//
//  Shop.h
//  Timo
//
//  Created by YUN on 14-12-30.
//
//

#ifndef __Timo__Shop__
#define __Timo__Shop__

#include <iostream>
#include "cocos2d.h"

using namespace std;
using namespace cocos2d;

#define BuyItemBgTag    1

const int ITEMNUM = 8;

class Shop : public CCLayer
{
public:
    Shop();
    ~Shop();
    
    static Shop* shared();
    
    static Shop* create(int kind = 0);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void exit(CCObject* obj);
    
private:
    void initBg();
    
    void buyGold(CCObject* obj);
    
    void removeTinyLoadingDelay();
    
private:
    CCMenu* itemMenu;
    bool    m_bItemMenu;
    
public:
    
    int m_pKind;
    
    bool m_bcantouch;
    
};

#endif /* defined(__Timo__Shop__) */
