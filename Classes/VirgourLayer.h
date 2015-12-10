//
//  VirgourLayer.h
//  coreball
//
//  Created by yang on 15/4/10.
//
//

#ifndef __coreball__VirgourLayer__
#define __coreball__VirgourLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "Funtion.h"

USING_NS_CC;

class VirgourLayer : public CCLayer
{
public:
    VirgourLayer(float pox,float poy);
    ~VirgourLayer();
    
    static VirgourLayer* shared();
    
    virtual bool init();
    
    static VirgourLayer* create(float pox,float poy);
    
    LayeronEnter(2);
    LayeronExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    void showVirgour();
    
    void updataVirgour();
    
    void showrelefttime(float dt);
    
    void openShop(CCObject* obj);
    
private:
    CCMenu* m_pMenu;
    bool    m_bMenu;
    
    CCNode* m_pNumNode;
    
    float m_pvirgourPox;
    float m_pvirgourPoy;
    
    int m_pCurVirgour;
    
    CCLabelTTF* m_pleftimettf;
    
};

#endif /* defined(__coreball__VirgourLayer__) */