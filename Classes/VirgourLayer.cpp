//
//  VirgourLayer.cpp
//  coreball
//
//  Created by yang on 15/4/10.
//
//

#include "VirgourLayer.h"
//#include "HelloWorldScene.h"
//#include "PauseGameLayer.h"
#include "Shop.h"

const int   TIMERUMECOUNT = 60*5; //體力回愎時間
const int   MAXVIRGOUR = 5;

#define KTAGOUTSIDELAY  101

static VirgourLayer* sharedInstance = NULL;

VirgourLayer* VirgourLayer::shared()
{
    return sharedInstance;
}
VirgourLayer::VirgourLayer(float pox,float poy)
:m_pvirgourPox(pox)
,m_pvirgourPoy(poy)
,m_pCurVirgour(readGameVirgour())
{
    sharedInstance = this;
}
VirgourLayer::~VirgourLayer()
{
    sharedInstance = NULL;
    this->unschedule(schedule_selector(VirgourLayer::updataVirgour));
}

VirgourLayer* VirgourLayer::create(float pox, float poy)
{
    VirgourLayer* lay = new VirgourLayer(pox,poy);
    if(lay && lay->init())
    {
        lay->autorelease();
        return lay;
    }
    
    CC_SAFE_DELETE(lay);
    return NULL;
}

bool VirgourLayer::init()
{
    if(!CCLayer::init())
        return false;
    
    m_bMenu = false;
    m_pMenu = CCMenu::create();
    m_pMenu->setPosition(CCPointZero);
    this->addChild(m_pMenu,12);
    
    m_pNumNode = CCNode::create();
    m_pNumNode->setPosition(CCPointZero);
    this->addChild(m_pNumNode,10);
    
    showVirgour();
    
    return true;
}

void VirgourLayer::showVirgour()
{
    CCSprite* virgourframe = createSprite("virgourFrame.png", m_pvirgourPox, m_pvirgourPoy);
    virgourframe->setAnchorPoint(ccp(0,0.5));
    this->addChild(virgourframe);
    
    CCSprite* virgourimage = CCSprite::create("virgourImage.png");
    virgourimage->setPosition(getacPos(m_pvirgourPox, m_pvirgourPoy));
    virgourimage->setScale(mWinScale);
    this->addChild(virgourimage);
    
    CCMenuItemImage* addvirbtn = createMenu("addBtn.png", "addBtn.png", this, menu_selector(VirgourLayer::openShop), m_pvirgourPox+virgourframe->getContentSize().width-20, m_pvirgourPoy);
    m_pMenu->addChild(addvirbtn);
    
    showOnePicNum("num/", m_pCurVirgour, m_pNumNode, m_pvirgourPox+50, m_pvirgourPoy,NUMBERLEFT);
    
    
    m_pleftimettf = createTTF(CCString::createWithFormat("体力回复： %02d:%02d",0,0)->getCString(), 40, m_pvirgourPox+virgourframe->getContentSize().width*0.5, m_pvirgourPoy+virgourframe->getContentSize().height*0.8);
    m_pleftimettf->setColor(ccBLACK);
    this->addChild(m_pleftimettf,10);
    
    this->schedule(schedule_selector(VirgourLayer::updataVirgour), 0.1);
    this->schedule(schedule_selector(VirgourLayer::showrelefttime), 0.15);
}

void VirgourLayer::openShop(cocos2d::CCObject *obj)
{
    CCNode* parnode = (this->getParent())->getParent();
    parnode->addChild(Shop::create(VirgourShop),300,KTAGOUTSIDELAY);
}

void VirgourLayer::updataVirgour()
{
    if(readGameVirgour() < MAXVIRGOUR)
    {
        if(getSystemTime()-readLastTime() >= TIMERUMECOUNT)
        {
            int curvirgour = readGameVirgour();
            curvirgour += (getSystemTime()-readLastTime())/TIMERUMECOUNT;
            if(curvirgour >= MAXVIRGOUR) curvirgour = MAXVIRGOUR;
            saveGameVirgour(curvirgour);
            
            saveLastTime();
        }
    }
    else
    {
        saveLastTime();
    }
    
    if(m_pCurVirgour != readGameVirgour())
    {
        m_pCurVirgour = readGameVirgour();
        m_pNumNode->removeAllChildren();
        showOnePicNum("num/", m_pCurVirgour, m_pNumNode, m_pvirgourPox+50, m_pvirgourPoy,NUMBERLEFT);
    }
    
}

void VirgourLayer::showrelefttime(float dt)
{
    if(readGameVirgour() < MAXVIRGOUR)
    {
        int min = (TIMERUMECOUNT-(getSystemTime()-readLastTime()))/60;
        int sec = (TIMERUMECOUNT-(getSystemTime()-readLastTime()))%60;
        m_pleftimettf->setString(CCString::createWithFormat("体力回复： %02d:%02d",min,sec)->getCString());
    }
    else
    {
        m_pleftimettf->setString(CCString::createWithFormat("体力回复： %02d:%02d",0,0)->getCString());
    }
}

bool VirgourLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
//    if((HelloWorld::shard() && HelloWorld::shard()->m_bgameover) || (PauseGameLayer::shard() && PauseGameLayer::shard()->m_bPause))
//        return false;
    
    m_bMenu = m_pMenu->ccTouchBegan(pTouch, pEvent);
    if(m_bMenu)
        return true;
    
    return false;
}
void VirgourLayer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(m_bMenu)
        m_pMenu->ccTouchMoved(pTouch, pEvent);
}
void VirgourLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(m_bMenu)
        m_pMenu->ccTouchEnded(pTouch, pEvent);
    
    m_bMenu = false;
}


